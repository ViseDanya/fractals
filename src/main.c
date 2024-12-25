#include "shader.h"

#include "SDL3/SDL.h"
#include <stdlib.h>
#include <math.h>

#define GL_SILENCE_DEPRECATION 1
#include <OpenGL/gl3ext.h>

#define WINDOW_WIDTH 800
#define WINDOW_HEIGHT 800
#define MAX_ITERATIONS 1000

static SDL_Window *window = NULL;
static SDL_GLContext context = NULL;

static GLfloat vertices[] = {
    -1,
    -1,
    0,
    1,
    -1,
    0,
    1,
    1,
    0,
    -1,
    1,
    0,
};

static GLuint vertexArrayObject, vertexBufferObject;
static GLuint shaderProgram;

void initSDL()
{
  if (!SDL_Init(SDL_INIT_VIDEO))
  {
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_Init failed: %s", SDL_GetError());
    SDL_Quit();
    exit(1);
  }

  window = SDL_CreateWindow("Fractals", WINDOW_WIDTH, WINDOW_HEIGHT, SDL_WINDOW_OPENGL);
  if (window == NULL)
  {
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_CreateWindow failed: %s", SDL_GetError());
    SDL_Quit();
    exit(1);
  }
}

void initOpenGL()
{
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MAJOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_MINOR_VERSION, 3);
  SDL_GL_SetAttribute(SDL_GL_CONTEXT_PROFILE_MASK, SDL_GL_CONTEXT_PROFILE_CORE);

  context = SDL_GL_CreateContext(window);
  if (context == NULL)
  {
    SDL_LogError(SDL_LOG_CATEGORY_ERROR, "SDL_GL_CreateContext failed: %s", SDL_GetError());
    SDL_Quit();
    exit(1);
  }

  glGenVertexArrays(1, &vertexArrayObject);
  glGenBuffers(1, &vertexBufferObject);

  glBindVertexArray(vertexArrayObject);
  glBindBuffer(GL_ARRAY_BUFFER, vertexBufferObject);
  glBufferData(GL_ARRAY_BUFFER, sizeof(vertices), vertices, GL_STATIC_DRAW);

  glVertexAttribPointer(0, 3, GL_FLOAT, GL_FALSE, 3 * sizeof(GLfloat), (void *)0);
  glEnableVertexAttribArray(0);

  shaderProgram = glCreateProgram();
  compileShaderProgram(shaderProgram, "../src/shaders/vertShader.glsl", "../src/shaders/fragShader.glsl");
}

void cleanup()
{
  SDL_GL_DestroyContext(context);
  SDL_DestroyWindow(window);
  SDL_Quit();
}

int main(int argc, char *argv[])
{
  initSDL();
  initOpenGL();

  bool quit = false;
  SDL_Event event;
  float zoom = 1.0;
  float mouseX, mouseY;
  float prevMouseX, prevMouseY = 0;
  float deltaX, deltaY = 0;
  float panX, panY = 0;

  float juliaX = 0;
  float juliaY = 0;
  bool pause = false;
  Uint32 prevTime = SDL_GetTicks();
  Uint32 currentTime = SDL_GetTicks();
  while (!quit)
  {
    while (SDL_PollEvent(&event) != 0)
    {
      if (event.type == SDL_EVENT_QUIT)
      {
        quit = true;
      }
      else if (event.type == SDL_EVENT_MOUSE_WHEEL)
      {
        zoom *= (event.wheel.y > 0) ? 1.1f : 0.9f; // Zoom in if scrolling up, zoom out if scrolling down
      }
      else if (event.type == SDL_EVENT_KEY_DOWN && event.key.key == SDLK_SPACE)
      {
        pause = !pause;
      }
    }

    prevMouseX = mouseX;
    prevMouseY = mouseY;
    if (SDL_GetMouseState(&mouseX, &mouseY) & SDL_BUTTON_LMASK)
    {
      deltaX = mouseX - prevMouseX;
      deltaY = mouseY - prevMouseY;
      panX -= deltaX / (WINDOW_WIDTH * zoom);
      panY += deltaY / (WINDOW_HEIGHT * zoom);
    }

    if (!pause)
    {
      Uint32 timeDiff = SDL_GetTicks() - prevTime;
      currentTime += timeDiff;
      juliaX = -.77;
      juliaY = (sin((float)(currentTime) / 1000) + 1) / 2 * .5 + -.25;
    }
    prevTime = SDL_GetTicks();

    // printf("%f\n", juliaY);
    glClearColor(0.f, 0.f, 0.f, 1.f);
    glClear(GL_COLOR_BUFFER_BIT);
    glUseProgram(shaderProgram);
    GLint juliaLocation = glGetUniformLocation(shaderProgram, "julia");
    glUniform2f(juliaLocation, juliaX, juliaY);
    GLint zoomLocation = glGetUniformLocation(shaderProgram, "zoom");
    glUniform1f(zoomLocation, zoom);
    GLint panLocation = glGetUniformLocation(shaderProgram, "pan");
    glUniform2f(panLocation, panX, panY);
    glBindVertexArray(vertexArrayObject);
    glDrawArrays(GL_TRIANGLE_FAN, 0, 4);

    SDL_GL_SwapWindow(window);
  }

  cleanup();
}
