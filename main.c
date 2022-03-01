
#include "syslayer/syslayer.h"
#include "standard.h"
#include "types.h"
#include "define.h"

#include "cvote_test.h"

typedef struct Global
{
  Bool is_running;
} Global;

Global* global = null;

Int
global_init(None)
{
  global = new(Global);
  if (global == null)
    {
      fprintf(stderr, "error, can't allocate global map\n");
      return error_out_of_mem;
    }
  memset(global, 0x0, sizeof(Global));
  
  return ok;
}

Int
global_terminate(None)
{
  if (global == null)
    {
      fprintf(stderr, "warn, global termination called twice\n");
      return warn_already;
    }

  free(global);
  return ok;
}

Int
read_events(None)
{
  Sysevent event;
  while (syslayer_read_event(&event) != event_none)
    {
      switch (event.type)
        {
        case event_quit:
          global->is_running = false;
          break;
        case event_keydown:
          if (event.key.code == key_q)
            global->is_running = false;
          break;
        }
    }
  return ok;
}

Int
test_all(None)
{
  fprintf(stdout, " > testing all\n");
  CVOTE_TEST_BEGIN();
  CVOTE_TEST(test_syslayer);
  CVOTE_TEST_END();
}

Int
main(None)
{
  GameColor background_color = (GameColor)
    { .r = 0x00, .g = 0xff, .b = 0xff, .a = 0xff };

  GameRect player_rect = (GameRect)
    { .x = 0x10, .y = 0x20, .w = 0x10, .h = 0x10 };
  GameColor player_color = (GameColor)
    { .r = 0xff, .g = 0x00, .b = 0x00, .a = 0xff };

#ifdef DEBUG
  if (test_all() != ok) return error_test;
#endif
  
  if (syslayer_init() < ok)
    {
      fprintf(stderr, " => can't init syslayer\n");
      return error_upper;
    }

  if (global_init() < ok)
    {
      fprintf(stderr, " => can't init global syslayer\n");
      return error_upper;
    }
  global->is_running = true;

  while (global != null && global->is_running == true)
    {
      syslayer_clear_window(background_color);

      syslayer_draw_rect(player_rect, player_color);
        
      syslayer_draw_window();
      read_events();
    }

  if (global != null) global_terminate();
  if (syslayer != null) syslayer_terminate();
  return ok;
}
