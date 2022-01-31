
#define DEBUG_MODE

#include <GL/gl.h>
#include <GLFW/glfw3.h>
#include <stdio.h>
#include <stdlib.h>

#include "types.h"
#include "error_codes.h"

#define _STR(X) #X
#define STR(X) _STR(X)

typedef struct GameSystem
{
  GLFWwindow* window;
} GameSystem;

GameSystem* game_system = null;

None
debug_startup_info(None)
{
  fprintf(stdout, "Compiled for GLFW version: %i.%i.%i\n",
          GLFW_VERSION_MAJOR,
          GLFW_VERSION_MINOR,
          GLFW_VERSION_REVISION);
  return;
}

None
glfw_error_handle(Int code, const char* message)
{
  fprintf(stderr, "glfw error:\n  code: %d\n  message: %s\n",
          code, message);
  return;
}

Int
game_system_init(None)
{
  if (game_system != null)
    {
      fprintf(stderr, "warning, " STR(__func__) " called more then one time\n");
      return error_already_done;
    }

  game_system = (GameSystem*) malloc(sizeof(GameSystem));
  if (game_system == null)
    {
      fprintf(stderr, "error, can't allocate memory for system struct\n");
      return error_out_of_mem;
    }

  glfwSetErrorCallback(glfw_error_handle);
  
  if (glfwInit() == 0x0)
    {
      fprintf(stderr, "error, can't init glfw\n");
      return error_lib;
    }
  
  game_system->window = glfwCreateWindow(640, 480, "Fun", null, null);
  if (game_system->window == null)
    {
      fprintf(stderr, "error, can't create window\n");
      glfwTerminate();
      return error_system;
    }

  glfwMakeContextCurrent(game_system->window);
  
  return error_none;
}

Int
game_system_terminate(None)
{
  if (game_system == null)
    {
      fprintf(stderr, "warning, tried to terminate uninitalized system\n");
      return error_already_done;
    }
  glfwDestroyWindow(game_system->window);
  glfwTerminate();
  return error_none;
}

GLfloat verts[] =
  {
    -0.5f, -0.5f,  0.0f,
    -0.5f,  0.5f,  0.0f,
     0.5f, -0.5f,  0.0f,
  };
GLuint VBO;
GLuint shader_vert;
char shader_vert_code[0x400];
FILE* shader_vert_fd = null;

Int
main(None)
{
  game_system_init();

#ifdef DEBUG_MODE
  debug_startup_info();
#endif /* DEBUG_MODE */

  glClearColor(0x00, 0xff, 0xff, 0xff);

  /* VBO init */
  glGenBuffers(1, &VBO);
  glBindBuffer(GL_ARRAY_BUFFER, VBO);
  
  glBufferData(GL_ARRAY_BUFFER, sizeof(verts), verts, GL_STATIC_DRAW);

  /* Vert shader */
  shader_vert = glCreateShader(GL_VERTEX_SHADER);
  shader_vert_fd = fopen("r", "./shader.vert"); /* TODO: Seperate shader pathes to defines */
  if (shader_vert_fd == null)
    {
      fprintf(stderr, "error, can't open vertex shader\n");
      game_system_terminate();
      return -0x1;
    }
  fgets(shader_vert_code, sizeof(shader_vert_code), shader_vert_fd);
  fclose(shader_vert_fd);
  glShaderSource(shader_vert, 1, &shader_vert_code, null);
  glCompileShader(shader_vert);
  
  while (glfwWindowShouldClose(game_system->window) != GLFW_TRUE)
    {
      glClear(GL_COLOR_BUFFER_BIT);

      
      glfwSwapBuffers(game_system->window);
      glfwPollEvents();
    }

  game_system_terminate();
  
  return error_none;
}

