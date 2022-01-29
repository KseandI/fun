
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


Int
game_system_init(None)
{
  if (game_system != null)
    {
      fprintf(stderr, "warning, " STR(__func__) " called more then one time\n");
      return error_already_done;
    }

  if (glfwInit() == 0x0)
    {
      fprintf(stderr, "error, can't init glfw\n");
      return error_lib;
    }

  game_system = (GameSystem*) malloc(sizeof(GameSystem));
  if (game_system == null)
    {
      fprintf(stderr, "error, can't allocate memory for system struct\n");
      return error_out_of_mem;
    }

  game_system->window = glfwCreateWindow(640, 480, "Fun", NULL, NULL);
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
  glfwTerminate();
  return error_none;
}

Int
main(None)
{
  game_system_init();

  while (glfwWindowShouldClose(game_system->window) != GLFW_TRUE)
    {
      glClear(GL_COLOR_BUFFER_BIT);
      glfwSwapBuffers(game_system->window);
      glfwPollEvents();
    }

  game_system_terminate();
  
  return error_none;
}

