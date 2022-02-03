
#define DEBUG_MODE
#define PRINT_GLFW_MESSAGES
#define PRINT_WARNING_MESSAGES
/*
#define PRINT_SHADERS
*/

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

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
#ifdef PRINT_GLFW_MESSAGES
  fprintf(stderr, "glfw error:\n  code: %d\n  message: %s\n",
          code, message);
#endif /* PRINT_GLFW_MESSAGES */
  return;
}

Int
game_system_init(None)
{
  if (game_system != null)
    {
#ifdef PRINT_WARNING_MESSAGES
      fprintf(stderr, "warning, " STR(__func__) " called more then one time\n");
#endif /* PRINT_WARNING_MESSAGES */
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

#ifdef DEBUG_MODE
  glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE);
#endif /* DEBUG_MODE */
  
  game_system->window = glfwCreateWindow(640, 480, "Fun", null, null);
  if (game_system->window == null)
    {
      fprintf(stderr, "error, can't create window\n");
      glfwTerminate();
      return error_system;
    }

  glfwMakeContextCurrent(game_system->window);

  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK)
    {
      fprintf(stderr, "error, can't init glew\n");
      return error_lib;
    }
  
  return error_none;
}

Int
game_system_terminate(None)
{
  if (game_system == null)
    {
#ifdef PRINT_WARNING_MESSAGES
      fprintf(stderr, "warning, tried to terminate uninitalized system\n");
#endif /* PRINT_WARNING_MESSAGES */
      return error_already_done;
    }
  
  glfwDestroyWindow(game_system->window);
  glfwTerminate();
  
  return error_none;
}

uint32_t
get_file_size(FILE* file)
{
  uint32_t current_pos;
  uint32_t result;
  if (file == null)
    {
#ifdef PRINT_WARNING_MESSAGES
      fprintf(stderr, "tried to get size of null-file\n");
#endif /* PRINT_WARNING_MESSAGES */
      return error_data;
    }
  current_pos = ftell(file);
  fseek(file, 0x0, SEEK_END);
  result = ftell(file);
  fseek(file, current_pos, SEEK_SET);
  return result;
}

GLuint
read_shader(GLenum type, const char* path)
{
  FILE* fd;
  char* source;
  uint32_t shader_size;
  GLuint shader;

  shader = glCreateShader(type);
  fd = fopen(path, "r");
  
  if (fd == null)
    {
#ifdef PRINT_WARNING_MESSAGES
      fprintf(stderr, "warning, can't open vertex shader \"%s\"\n", path);
#endif /* PRINT_WARNING_MESSAGES */
      
    shader_fallback:
      source = "";
      shader_size = 0x0;
    }
  else
    {
      shader_size = get_file_size(fd);
      
      source = (char*) malloc(sizeof(char)*shader_size);
      memset(source, 0x0, shader_size);
      
      if (source == null)
        {
          fprintf(stderr, "error, can't allocate memory for shader\n");
          goto shader_fallback;
        }
      
      /* It just works =| */
      while (fgets(source+strlen(source), shader_size, fd) != 0x0) {}
      
      fclose(fd);
    }
  
#ifdef PRINT_SHADERS
  fprintf(stdout, "Shader(%u): \"%s\"\n", shader_size, source);
#endif /* PRINT_SHADERS */
  
  glShaderSource(shader, 1, (const char**) &source, null);
  glCompileShader(shader);

  return shader;
}

GLuint shader_program,
  shader_frag,
  shader_vert;

Int
main(None)
{
  /* init render system */
  if (game_system_init() < error_none)
    {
      fprintf(stderr, " => error, can't init game system\n");
      return error_upper;
    }

  /* print debug info */
#ifdef DEBUG_MODE
  debug_startup_info();
#endif /* DEBUG_MODE */

  /* set clear color */
  glClearColor(0x00, 0xff, 0xff, 0xff);

  /* Shaders */
  shader_vert = read_shader(GL_VERTEX_SHADER, "./shader.vert");
  shader_frag = read_shader(GL_FRAGMENT_SHADER, "./shader.frag");

  /* Shader program */
  shader_program = glCreateProgram();
  
  glAttachShader(shader_program, shader_vert);
  glAttachShader(shader_program, shader_frag);
  
  glLinkProgram(shader_program);
  
  glDeleteShader(shader_vert);
  glDeleteShader(shader_frag);

  /* main loop */
  while (glfwWindowShouldClose(game_system->window) != GLFW_TRUE)
    {
      /* clear background */
      glClear(GL_COLOR_BUFFER_BIT);

      /* print changes */
      glfwSwapBuffers(game_system->window);
      /* get all events from system */
      glfwPollEvents();
    }

  /* terminate all render systems */
  game_system_terminate();
  
  return error_none;
}

/* TODO list
 * * add shader return code
 * * seperate error log into macros
 *
 */


