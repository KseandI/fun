
#ifndef GAME_SYSTEM_INC
#define GAME_SYSTEM_INC

#include "standard.h"
#include "defines.h"
#include "types.h"

#define SDL_DISABLE_IMMINTRIN_H
#include <SDL2/SDL_keycode.h>

typedef enum KEYS
  {
    KEY_NONE = 0x0,
    KEY_UP = SDL_SCANCODE_UP,
    KEY_RIGHT = SDL_SCANCODE_RIGHT,
    KEY_DOWN = SDL_SCANCODE_DOWN,
    KEY_LEFT = SDL_SCANCODE_LEFT,
    KEYS_LEN = SDL_NUM_SCANCODES
  } KEYS;

typedef struct SystemLayer
{
  Bool is_render_inited;
  Ptr window;
  Ptr render;
  Bool* keys;
} SystemLayer;

typedef struct RenderObject
{
  GameRect rect;
  GameColor color;
} RenderObject;

typedef struct GameSystem
{
  Bool is_running;
  UInt objects_len;
  RenderObject** objects; /* array of pointers */
} GameSystem;

extern SystemLayer* syslayer;
extern GameSystem* gamesystem;

Int syslayer_init(None);
Int syslayer_terminate(None);
Int syslayer_get_events(None);
Int syslayer_clear_window(GameColor color);
Int syslayer_draw_window(None);
Int syslayer_draw_rect(GameRect rect, GameColor color);

Int game_init(None);
Int game_terminate(None);
Int game_user_terminate(None);
Int game_process_events(None);
RenderObject* game_create_object(None);
Int game_draw_object(RenderObject* object);
Int game_draw_objects(None);

#endif /* GAME_SYSTEM_INC */
