
#ifndef GAME_SYSTEM_INCLUDE
#define GAME_SYSTEM_INCLUDE

#include "standard.h"
#include "types.h"
#include "define.h"
#include "cvote_test.h"

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

typedef struct RenderObject
{
  GameRect rect;
  GameColor color;
} RenderObject;

typedef struct RenderPipeline
{
  GameColor background;
  UInt pipeline_len;
  RenderObject* render_objects;
} RenderPipeline;

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
Int syslayer_draw_rect(GameRect rect, GameColor color);

RenderObject* syslayer_create_renderobject(None);

RenderPipeline* syslayer_create_pipeline(None);
Int syslayer_set_pipeline_background(RenderPipeline* pipeline,
                                     GameColor color);
Int syslayer_pipeline_add_object(RenderPipeline* pipeline,
                                 RenderObject* object);
Int syslayer_pipeline_frame(RenderPipeline* pipeline);

/* testing */
Test test_syslayer_init(None);
Test test_syslayer_terminate(None);
Test test_syslayer(None);

#endif /* GAME_SYSTEM_INCLUDE */
