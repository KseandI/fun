
#include "system/standard.h"
#include "system/types.h"
#include "system/defines.h"
#include "system/system.h"


Int
main(None)
{
  GameColor background_color = (GameColor)
    { .r = 0x00, .g = 0xff, .b = 0xff, .a = 0xff, };
  RenderObject* player;
  Float player_speed = 0x0.1p-1;
  
  if (syslayer_init() < ok)
    {
      fprintf(stderr, "error, can't init system layer\n");
      return error_upper;
    }
  if (game_init() < ok)
    {
      fprintf(stderr, "error, can't init game\n");
      return error_upper;
    }

  player = game_create_object();
  
  gamesystem->is_running = true;

  while (gamesystem->is_running == true)
    {
      Vector2 wish_dir;
      
      syslayer_clear_window(background_color);

      wish_dir.x = syslayer->keys[KEY_RIGHT] - syslayer->keys[KEY_LEFT];
      wish_dir.y = syslayer->keys[KEY_DOWN] - syslayer->keys[KEY_UP];
      player->rect.x += wish_dir.x * player_speed;
      
      game_draw_objects();
      
      syslayer_draw_window();
      game_process_events();
    }

  return ok;
}

