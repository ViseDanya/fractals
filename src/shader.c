#include "shader.h"
#include "SDL3/SDL.h"
#include <stdio.h>
#include <stdlib.h>

int compileShader(GLuint shader, const GLchar *shaderSource);
int readShader(char *fileName, GLchar **program);

int compileShaderProgram(GLuint shaderProgram, char *vertexShaderFileName, char *fragmentShaderFileName)
{
    GLuint vertexShader = glCreateShader(GL_VERTEX_SHADER);
    GLchar *vertexShaderSource;
    readShader(vertexShaderFileName, &vertexShaderSource);
    compileShader(vertexShader, vertexShaderSource);
    free(vertexShaderSource);


    GLuint fragmentShader = glCreateShader(GL_FRAGMENT_SHADER);
    GLchar *fragmentShaderSource;
    readShader(fragmentShaderFileName, &fragmentShaderSource);
    compileShader(fragmentShader, fragmentShaderSource);
    free(fragmentShaderSource);

    glAttachShader(shaderProgram, vertexShader);
    glAttachShader(shaderProgram, fragmentShader);
    glLinkProgram(shaderProgram);

    int success;
    char infoLog[512];
    glGetProgramiv(shaderProgram, GL_LINK_STATUS, &success);
    if (!success)
    {
        glGetProgramInfoLog(shaderProgram, 512, NULL, infoLog);
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Linking shader program failed\n%s\n", infoLog);
        return 1;
    }

    glDeleteShader(vertexShader);
    glDeleteShader(fragmentShader);
    return 0;
}

int compileShader(GLuint shader, const GLchar *shaderSource)
{
    glShaderSource(shader, 1, &shaderSource, NULL);
    glCompileShader(shader);

    int success;
    char infoLog[512];
    glGetShaderiv(shader, GL_COMPILE_STATUS, &success);
    if (!success)
    {
        glGetShaderInfoLog(shader, 512, NULL, infoLog);
        int shaderType;
        glGetShaderiv(shader, GL_SHADER_TYPE, &shaderType);
        SDL_LogError(SDL_LOG_CATEGORY_ERROR, "Shader compilation failed. Shader Type: %s.\n%s\n", 
        shaderType == GL_VERTEX_SHADER ? "Vertex Shader" : "Fragment Shader", infoLog);
        return 1;
    }

    return 0;
}

int readShader(char *fileName, GLchar **program)
{
    FILE *fp;
    char buffer[255];

    // Open the file for reading
    fp = fopen(fileName, "r");

    // Check if the file was opened successfully
    if (fp == NULL)
    {
        perror("Error opening file!\n");
        return 1;
    }

    if (fseek(fp, 0L, SEEK_END) != 0)
    {
        perror("Error seeking to end of file!\n");
        return 1;
    }

    long fileSize = ftell(fp);
    if (fileSize == -1)
    {
        perror("Error getting file size!\n");
        return 1;
    }

    *program = malloc(sizeof(char) * (fileSize+1));

    if (fseek(fp, 0L, SEEK_SET) != 0)
    {
        perror("Error seeking to start of file!\n");
        return 1;
    }

    size_t readSize = fread(*program, sizeof(char), fileSize, fp);
    if (ferror(fp) != 0)
    {
        perror("Error reading file!");
        return 1;
    }
    else
    {
        (*program)[readSize++] = '\0';
    }
    fclose(fp);
    return 0;
}