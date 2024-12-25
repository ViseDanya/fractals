#ifndef SHADER_H
#define SHADER_H

#define GL_SILENCE_DEPRECATION 1
#include <OpenGL/gl3.h>

int compileShaderProgram(GLuint shaderProgram, char* vertexShaderFileName, char* fragmentShaderFileName);
#endif