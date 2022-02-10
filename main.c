
#include "internal.h"

#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

#include <stdio.h>
#include <stdlib.h>
#include <string.h>

#include "types.h"
#include "preproc_tools.h"
#include "error_codes.h"

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
game_reload_wireframe_mode(None)
{
  if (game_options == null || game_system == null) /* if not inited game */
    return ;

  if (game_options->wireframe_mode != 0x0)
    glPolygonMode(GL_FRONT_AND_BACK, GL_LINE); /* don't fill polygons */
  else
    glPolygonMode(GL_FRONT_AND_BACK, GL_FILL); /* fill polygons */

  return ;
}

None
game_reload_all(None)
{
  game_reload_wireframe_mode();
  return ;
}

None
game_toggle_wireframe_mode(None)
{
  if (game_options == null)     /* if options not inited */
    return ;
  if (game_system == null)      /* if glew not inited */
    return ;

  game_options->wireframe_mode = !(game_options->wireframe_mode); /* toggle options */

  game_reload_wireframe_mode();

  return ;
}

void
glfw_error_handle(int code, const char* message)
{
#ifdef PRINT_GLFW_MESSAGES
  PRT_ERROR("glfw error:\n  code: %d\n  message: %s\n",
          code, message);
#endif /* PRINT_GLFW_MESSAGES */
  return;
}

void
glfw_key_handle(GLFWwindow* win, int key, int scancode, int action, int mods)
{
  if (game_system == null || game_options == null)      /* if game not inited */
    return ;
  if (win != game_system->window) /* if inputed on another window */
    return ;

  if (action == GLFW_PRESS)
    {
      switch (key)
        {
        case GLFW_KEY_W:        /* toggle wireframe mode */
          game_toggle_wireframe_mode();
          break;
        case GLFW_KEY_Q:        /* quit game */
          game_options->is_game_running = false;
          break;
        default:
          break;
        }
      return;
    }

  return ;
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

  glfwSetErrorCallback(glfw_error_handle); /* set function/callback for glfw error handle */

  if (glfwInit() == 0x0)        /* init */
    {
      PRT_ERROR("error, can't init glfw\n");
      return error_lib;
    }

  ON_DEBUG(glfwWindowHint(GLFW_OPENGL_DEBUG_CONTEXT,
                          GL_TRUE)); /* enable opengl debug context (IDK what it is) */

  game_system->window = glfwCreateWindow(640, 480, "Fun", null, null); /* create window */
  if (game_system->window == null)
    {
      PRT_ERROR("error, can't create window\n");
      glfwTerminate();
      return error_system;
    }

  glfwMakeContextCurrent(game_system->window); /* select context */
  glfwSetKeyCallback(game_system->window, glfw_key_handle); /* set function for key input handle */

  glewExperimental = GL_TRUE;   /* enable experimental function */
  if (glewInit() != GLEW_OK)    /* get new opengl functions */
    {
      PRT_ERROR("error, can't init glew\n");
      return error_lib;
    }

  glViewport(0, 0, 640, 480);   /* pass window size to opengl */

  return error_none;
}

Int
game_system_terminate(None)
{
  if (game_system == null)      /* if system already terminated */
    {
      PRT_WARN("warning, tried to terminate uninitalized system\n");
      return error_already_done;
    }

  glfwDestroyWindow(game_system->window); /* terminate main window */
  glfwTerminate();                        /* terminate glfw render */

  free(game_system);            /* de-allocate system struct */

  return error_none;
}

Int
game_options_init(None)
{
  game_options = new(GameOptions); /* allocate new options */
  if (game_options == null)        /* if out of memory */
    {
      PRT_ERROR("error, can't allocate memory for options (rly?)\n");
      return error_out_of_mem;
    }
  memset(game_options, 0x0, sizeof(GameOptions)); /* clear structure */

  return error_none;
}

Int
game_options_terminate(None)
{
  if (game_options == null)     /* if already deallocated */
    {
      PRT_WARN("warning, tried to terminate uninitalized options\n");
      return error_already_done;
    }
  free(game_options);           /* deallocate options */
  return error_none;
}

UInt
get_file_size(FILE* file)
{
  UInt current_pos;
  UInt result;
  if (file == null)             /* if file doesn't exists */
    {
      PRT_DEBUG("tried to get size of null-file\n");
      return error_data;
    }
  current_pos = ftell(file);    /* get current cursor position */
  fseek(file, 0x0, SEEK_END);   /* go to the end */
  result = ftell(file);         /* get cursor position */
  fseek(file, current_pos, SEEK_SET); /* return to the original position */
  return result;
}

GLuint
read_shader(GLenum type, const char* path)
{
  Int error_code;               /* compilation error code */
  Char error_message[0x200];    /* compilation error message */
  FILE* fd;                     /* file descriptor (system's file abstraction) */
  char* source;                 /* shader's code */
  UInt shader_size;             /* size of shader */
  GLuint shader;                /* pointer to the shader */

  shader = glCreateShader(type); /* create new shader */
  fd = fopen(path, "r");         /* open source file */

  if (fd == null)               /* if file doesn't exists */
    {
      PRT_WARN("warning, can't open vertex shader \"%s\"\n", path);

    shader_fallback:            /* use empty shader */
      source = "#version 330 core\n";
      shader_size = 0x0;
    }
  else
    {
      shader_size = get_file_size(fd); /* get shader size */

      source = (char*) malloc(sizeof(char)*shader_size); /* allocate memory for shader source */

      if (source == null)       /* if can't allocate memory */
        {
          PRT_ERROR("error, can't allocate memory for shader\n");
          goto shader_fallback;
        }
      memset(source, 0x0, shader_size);                  /* clear this memory */

      /* It just works =| */
      while (fgets(source+strlen(source), shader_size, fd) != 0x0) {} /* read shader to the string */

      fclose(fd);               /* close file */
    }

#ifdef PRINT_SHADERS
  PRT_DEBUG("Shader(%u): \"%s\"\n", shader_size, source); /* print shader source if required */
#endif /* PRINT_SHADERS */

  glShaderSource(shader, 1, (const char**) &source, null); /* copy shader's source to the gpu */
  glCompileShader(shader);                                 /* compile shader */

  glGetShaderiv(shader, GL_COMPILE_STATUS, &error_code); /* get compilation result */
  if (error_code == 0x0)                                 /* if result isn't ok */
    {
      glGetShaderInfoLog(shader, 0x200, null, error_message); /* get logs */
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

  prog = glCreateProgram();     /* create opengl program */

  glAttachShader(prog, vert);   /* attach two shaders */
  glAttachShader(prog, frag);

  glLinkProgram(prog);          /* link shaders in program */

  glGetProgramiv(prog, GL_LINK_STATUS, &error_code); /* get link status */
  if (error_code == 0x0)                             /* if status isn't ok */
    {
      glGetProgramInfoLog(prog, 0x200, null, error_message); /* get error message */
      PRT_ERROR("error, can't link shaders to program\n"
                "log: %s\n", error_message);
      return 0x0;
    }


  return prog;
}

Vao*
vao_create(None)
{
  Vao* vao;

  vao = new(Vao);
  if (vao == null)
    {
      PRT_ERROR("error, can't allocate memory for vao\n");
      return null;
    }
  memset(vao, 0x0, sizeof(Vao));

  glGenVertexArrays(1, &(vao->ptr)); /* generate vao on gpu */
  vao_bind(vao);
  return vao;
}

Int
vao_bind(Vao* vao)
{
  if (vao == null)
    glBindVertexArray(null);
  else
    glBindVertexArray(vao->ptr);
  return error_none;
}

Int
vao_add(Vao* vao, UInt size, Ptr data)
{
  GLuint vbo;

  if (vao == null)
    {
      PRT_ERROR("error, tring to add data into unexisted vao\n");
      return error_null;
    }
  vao_bind(vao);

  glGenBuffers(1, &vbo);
  glBindBuffer(GL_ARRAY_BUFFER, vbo);
  glBufferData(GL_ARRAY_BUFFER, size*sizeof(GL_FLOAT), data, GL_STATIC_DRAW);
  glVertexAttribPointer(vao->len, size, GL_FLOAT, GL_FALSE, 0, null);

  vao->vbos[vao->len++] = vbo;
  return error_none;
}

Int
vao_draw(Vao* vao, GLsizei count)
{
  if (vao == null)
    {
      PRT_ERROR("error, can't draw null-vao\n");
      return error_null;
    }

  vao_bind(vao);

  for (UInt i = 0x0; i < vao->len; ++i)
    glEnableVertexAttribArray(i);
  glDrawArrays(GL_TRIANGLES, 0, count);
  for (UInt i = 0x0; i < vao->len; ++i)
    glDisableVertexAttribArray(i);
}

GLuint
shader_program_new_unique(GLuint vert, GLuint frag)
{
  GLuint prog;

  if ((prog = shader_program_new(vert, frag)) == 0x0) /* if can't create program */
    {
      PRT_ERROR(" => error, can't create shader program\n");
      return 0x0;
    }

  glDeleteShader(vert);         /* delete two shaders that already was copied */
  glDeleteShader(frag);

  return prog;
}


Int
main(None)
{
  GLuint shader_program,
    shader_frag,
    shader_vert;
  Vao* vao;
  uint32_t ebo;

  if (game_system_init() < error_none) /* init system */
    {
      PRT_ERROR(" => error, can't init game system\n");
      return error_upper;
    }

  if (game_options_init() < error_none) /* init options */
    {
      PRT_ERROR(" => error, can't init game options\n");
      return error_upper;
    }

  ON_DEBUG(debug_startup_info()); /* print debug info */

  glClearColor(0x00, 0xff, 0xff, 0xff); /* set clear color */

  /* read shaders */
  shader_vert = read_shader(GL_VERTEX_SHADER, "./shader.vert");
  shader_frag = read_shader(GL_FRAGMENT_SHADER, "./shader.frag");

  shader_program = shader_program_new_unique(shader_vert, shader_frag); /* shader program */
  if (shader_program == 0x0)
    {
      PRT_ERROR(" => error, can't use shaders\n");
    }


  GLfloat verts[] =
    {
      -1.0f,  0.0f,  0.0f, /* UL */
      +1.0f,  0.0f,  0.0f, /* UR */
      +1.0f, -1.0f,  0.0f, /* DR */

      -1.0f, -1.0f,  0.0f, /* DL */
    };

  GLuint indices[] =
    {
      0x0, 0x1, 0x2,
      0x2, 0x3, 0x0
    };

  /* arrays */
  vao = vao_create();
  vao_add(vao, sizeof(verts), verts);

  glGenBuffers(1, &ebo);        /* generate ebo */
  glBindBuffer(GL_ELEMENT_ARRAY_BUFFER, ebo); /* bind vao element array to ebo */
  glBufferData(GL_ELEMENT_ARRAY_BUFFER,
               sizeof(indices),
               indices,
               GL_STATIC_DRAW); /* copy indices to ebo */

  game_options->wireframe_mode = 0x1; /* enable wireframe mode */
  game_options->is_game_running = 0x1; /* start game */

  game_reload_all();            /* apply all options */

  /* main loop */
  while (game_options->is_game_running)
    {
      game_options->is_game_running &= /* simple, if not is_run == true
                                          and (should_close == false) then exit */
        glfwWindowShouldClose(game_system->window) == GLFW_FALSE;

      glClear(GL_COLOR_BUFFER_BIT); /* clear background */

      /* main code */
      glUseProgram(shader_program); /* use shader program */
      vao_draw(vao, 0x3);

      glfwSwapBuffers(game_system->window); /* Print changes */
      glfwPollEvents();         /* Get all events from system */
    }

  game_options_terminate();     /* terminate allocated options for game */
  game_system_terminate();      /* terminate all render systems */

  return error_none;
}
