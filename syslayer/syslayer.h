
#ifndef GAME_SYSTEM_INCLUDE
#define GAME_SYSTEM_INCLUDE

#include "standard.h"
#include "types.h"
#include "define.h"

typedef enum EventType
  {
    event_none = 0x0,
    event_quit,
    event_keydown,
    event_keyup,
    EventType_len
  } EventType;


#include <SDL2/SDL_keycode.h>

typedef enum Syskey
  {
    key_none = 0x0,
    key_q = SDL_SCANCODE_Q,
    keys_len = SDL_NUM_SCANCODES
  } Syskey;

typedef struct EventKey
{
  Syskey code;
} EventKey;

typedef struct Sysevent
{
  EventType type;
  EventKey key;
} Sysevent;

typedef struct Syslayer
{
  Ptr window;
  Ptr render;
} Syslayer;

extern Syslayer* syslayer;

Int syslayer_init(None);
Int syslayer_terminate(None);
EventType syslayer_read_event(Sysevent* event);
Int syslayer_clear_window(GameColor color);
Int syslayer_draw_window(None);

#endif /* GAME_SYSTEM_INCLUDE */
