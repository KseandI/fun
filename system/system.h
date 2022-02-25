
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


extern SystemLayer* syslayer;

Int syslayer_init(None);
Int syslayer_terminate(None);
Int syslayer_get_events(None);
Int syslayer_clear_window(GameColor color);
Int syslayer_draw_window(None);
Int syslayer_draw_rect(GameRect rect, GameColor color);

#endif /* GAME_SYSTEM_INC */
