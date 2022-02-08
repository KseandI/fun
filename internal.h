
/* header guard */
#ifndef INTERNAL_INC
#define INTERNAL_INC

/*
 * Includes
 */
/* opengl libs */
#define GLEW_STATIC
#include <GL/glew.h>
#include <GLFW/glfw3.h>

/* standard libs */
#include <stdint.h>
#include <stdio.h>

/* project libs */
#include "types.h"
#include "preproc_tools.h"
#include "error_codes.h"

/*
 * defines
 */

#define DEBUG_MODE
#define PRINT_GLFW_MESSAGES
#define PRINT_WARNING_MESSAGES
/*
#define PRINT_SHADERS
*/

/*
 * types
 */

typedef struct Vec2
{
  Float x, y;
} Vec2;

typedef struct Vao
{
  GLuint ptr;
} Vao;

typedef struct Vbo
{
  GLuint ptr;
  UInt size;
} Vbo;

typedef struct GameSystem
{
  GLFWwindow* window;
} GameSystem;

typedef struct GameOptions
{
  Bool is_game_running;
  Bool dev_mode;
  Bool wireframe_mode;
} GameOptions;

typedef struct RenderObject
{
  Vec2 transform;
  GLuint vao;
} RenderObject;

/*
 * variables
 */

GameSystem* game_system = null;
GameOptions* game_options = null;

/*
 * functions
 */

None debug_startup_info(None);
None game_reload_wireframe_mode(None);
None game_reload_all(None);
None game_toggle_wireframe_mode(None);
void glfw_error_handle(int code, const char* message);
void glfw_key_handle(GLFWwindow* win, int key, int scancode, int action, int mods);
Int game_system_init(None);
Int game_system_terminate(None);
Int game_options_init(None);
Int game_options_terminate(None);
UInt get_file_size(FILE* file);
GLuint read_shader(GLenum type, const char* path);
GLuint shader_program_new(GLuint vert, GLuint frag);
GLuint shader_program_new_unique(GLuint vert, GLuint frag);
Vao* vao_create(None);
Int vao_bind(Vao* vao);
Vbo* vbo_create(None);
Int vbo_bind(Vbo* vbo);
Int vbo_load_data(Vbo* vbo, Ptr data, UInt size, GLenum type);
Int main(None);

#endif /* INTERNAL_INC */
