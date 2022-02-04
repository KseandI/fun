
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
#include "preproc_tools.h"
#include "error_codes.h"

typedef struct GameSystem
{
  GLFWwindow* window;
} GameSystem;

typedef struct GameOptions
{
  Bool dev_mode;
  Bool wireframe_mode;
} GameOptions;

GameSystem* game_system = null;
GameOptions* game_options = null;

None
debug_startup_info(None)
{
  PRT_DEBUG("Compiled for GLFW version: %i.%i.%i\n",
            GLFW_VERSION_MAJOR,
            GLFW_VERSION_MINOR,
            GLFW_VERSION_REVISION);
  return;
}

None
glfw_error_handle(Int code, const char* message)
{
#ifdef PRINT_GLFW_MESSAGES
  PRT_ERROR("glfw error:\n  code: %d\n  message: %s\n",
          code, message);
#endif /* PRINT_GLFW_MESSAGES */
  return;
}

Int
game_system_init(None)
{
  if (game_system != null)
    {
      PRT_WARN("warning, " STR(__func__) " called more then one time\n");
      return error_already_done;
    }

  game_system = (GameSystem*) malloc(sizeof(GameSystem));
  if (game_system == null)
    {
      PRT_ERROR("error, can't allocate memory for system struct\n");
      return error_out_of_mem;
    }

  glfwSetErrorCallback(glfw_error_handle);
  
  if (glfwInit() == 0x0)
    {
      PRT_ERROR("error, can't init glfw\n");
      return error_lib;
    }

  ON_DEBUG(glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT, GL_TRUE));
  
  game_system->window = glfwCreateWindow(640, 480, "Fun", null, null);
  if (game_system->window == null)
    {
      PRT_ERROR("error, can't create window\n");
      glfwTerminate();
      return error_system;
    }

  glfwMakeContextCurrent(game_system->window);

  glewExperimental = GL_TRUE;
  if (glewInit() != GLEW_OK)
    {
      PRT_ERROR("error, can't init glew\n");
      return error_lib;
    }
  
  return error_none;
}

Int
game_system_terminate(None)
{
  if (game_system == null)
    {
      PRT_WARN("warning, tried to terminate uninitalized system\n");
      return error_already_done;
    }
  
  glfwDestroyWindow(game_system->window);
  glfwTerminate();

  free(game_system);
  
  return error_none;
}

Int
game_options_init(None)
{
  game_options = new(GameOptions);
  if (game_options == null)
    {
      PRT_ERROR("error, can't allocate memory for options (rly?)\n");
      return error_out_of_mem;
    }
  memset(game_options, 0x0, sizeof(GameOptions));

  return error_none;
}

Int
game_options_terminate(None)
{
  if (game_options == null)
    {
      PRT_WARN("warning, tried to terminate uninitalized options\n");
      return error_already_done;
    }
  free(game_options);
  return error_none;
}

uint32_t
get_file_size(FILE* file)
{
  uint32_t current_pos;
  uint32_t result;
  if (file == null)
    {
      PRT_DEBUG("tried to get size of null-file\n");
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
  Int error_code;
  Char error_message[0x200];
  FILE* fd;
  char* source;
  uint32_t shader_size;
  GLuint shader;

  shader = glCreateShader(type);
  fd = fopen(path, "r");
  
  if (fd == null)
    {
      PRT_WARN("warning, can't open vertex shader \"%s\"\n", path);
      
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
          PRT_ERROR("error, can't allocate memory for shader\n");
          goto shader_fallback;
        }
      
      /* It just works =| */
      while (fgets(source+strlen(source), shader_size, fd) != 0x0) {}
      
      fclose(fd);
    }
  
#ifdef PRINT_SHADERS
  PRT_DEBUG("Shader(%u): \"%s\"\n", shader_size, source);
#endif /* PRINT_SHADERS */
  
  glShaderSource(shader, 1, (const char**) &source, null);
  glCompileShader(shader);

  glGetShaderiv(shader, GL_COMPILE_STATUS, &error_code);
  if (error_code == 0x0)
    {
      PRT_ERROR("error, can't compile shader %s\n"
                "log: %s\n", path, error_message);
      return 0x0;
    }

  return shader;
}

GLuint
shader_program_new(GLuint vert, GLuint frag)
{
  Int error_code;
  Char error_message[0x200];
  GLuint prog;
  
  prog = glCreateProgram();

  glAttachShader(prog, vert);
  glAttachShader(prog, frag);

  glLinkProgram(prog);

  glGetProgramiv(prog, GL_LINK_STATUS, &error_code);
  if (error_code == 0x0)
    {
      glGetProgramInfoLog(prog, 0x200, null, error_message);
      PRT_ERROR("error, can't link shaders to program\n"
                "log: %s\n", error_message);
      return 0x0;
    }
      

  return prog;
}

GLuint
shader_program_new_unique(GLuint vert, GLuint frag)
{
  GLuint prog;
  
  prog = shader_program_new(vert, frag);
  
  glDeleteShader(vert);
  glDeleteShader(frag);
  
  return prog;
}


Int
main(None)
{
  GLuint shader_program,
    shader_frag,
    shader_vert;
  uint32_t vao, vbo, ebo;
  
  /* init render system */
  if (game_system_init() < error_none)
    {
      PRT_ERROR(" => error, can't init game system\n");
      return error_upper;
    }

  if (game_options_init() < error_none)
    {
      PRT_ERROR(" => error, can't init game options\n");
      return error_upper;
    }

  ON_DEBUG(debug_startup_info()); /* print debug info */

  glClearColor(0x00, 0xff, 0xff, 0xff); /* set clear color */

  /* Shaders */
  shader_vert = read_shader(GL_VERTEX_SHADER, "./shader.vert");
  shader_frag = read_shader(GL_FRAGMENT_SHADER, "./shader.frag");

  shader_program = shader_program_new_unique(shader_vert, shader_frag); /* shader program */

  SFloat vert_pos[] =
    {
      -1.0f,  0.0f,  0.0f, /* UL */
      +1.0f,  0.0f,  0.0f, /* UR */
      +1.0f, -1.0f,  0.0f, /* DR */
       
      -1.0f, -1.0f,  0.0f, /* DL */
    };

  uint32_t indices[] =
    {
      0x0, 0x1, 0x2,
      0x2, 0x3, 0x0
    };
  
  /* arrays */
  glGenVertexArrays(1, &vao);   /* generate vao on card */
  glBindVertexArray(vao);       /* bind current vao to created */
  glGenBuffers(1, &vbo);        /* generate one vbo */
  glBindBuffer(GL_ARRAY_BUFFER, vbo); /* bind current buffer to this buffer */
  glBufferData(GL_ARRAY_BUFFER, sizeof(vert_pos), vert_pos, GL_STATIC_DRAW); /* copy data from vert_pos to static memory on card */
  glVertexAttribPointer(0x0, 0x3, GL_FLOAT, GL_FALSE, 0x3 * sizeof(SFloat), null); /* create information about buffer */
  glEnableVertexAttribArray(0); /* activate buffer */

  glGenBuffers(1, &ebo);        /* generate ebo */
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo); /* bind vao element array to ebo */
  glBufferData(GL_ELEMENT_ARRAY_BUFFER, sizeof(indices), indices, GL_STATIC_DRAW); /* copy indices to ebo */
  
  
  game_options->wireframe_mode = 0x1; /* enable wireframe mode */

  /* main loop */
  while (glfwWindowShouldClose(game_system->window) != GLFW_TRUE)
    {
      glClear(GL_COLOR_BUFFER_BIT); /* clear background */

      /* main code */
      glUseProgram(shader_program);
      glBindVertexArray(vao);
      
      /* control wireframe mode */
      if (game_options->wireframe_mode != 0x0)
        glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); /* don't fill polygons */
      else
        glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); /* fill polygons */
      
      glDrawElements(GL_TRIANGLES, 0x6, GL_UNSIGNED_INT, 0x0);
      
      glfwSwapBuffers(game_system->window); /* Print changes */
      glfwPollEvents();         /* Get all events from system */
    }

  game_options_terminate();     /* terminate allocated options for game */
  game_system_terminate();      /* terminate all render systems */
  
  return error_none;
}


