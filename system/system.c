
#include "standard.h"
#include "defines.h"
#include "types.h"
#include "in_system.h"
#include "system.h"

SystemLayer* syslayer = null;
GameSystem* gamesystem = null;

Int
syslayer_init(None)
{
  /* === create system layer === */
  syslayer = new(SystemLayer);
  if (syslayer == null)
    {
      fprintf(stderr, "error, can't allocate memory\b"
              "for system layer\n");
      return error_out_of_mem;
    }         
  memset(syslayer, 0x0, sizeof(SystemLayer));


  /* === init render === */
  if (SDL_Init(SDL_INIT_VIDEO) < 0x0)
    {
      fprintf(stderr,
              "error, can't init sdl\n"
              "sdl log: %s\n", SDL_GetError());
      return error_render;
    }
  /* Create 640x480 resizable window with title "fun" */
  syslayer->window = SDL_CreateWindow("fun",
                                      SDL_WINDOWPOS_UNDEFINED,
                                      SDL_WINDOWPOS_UNDEFINED,
                                      0x280, 0x1e0,
                                      SDL_WINDOW_RESIZABLE);
  if (syslayer->window == null)
    {
      fprintf(stderr, "error, can't create window\n");
      return error_render;
    }

  /* Create gpu accelerated render on window */
  syslayer->render = SDL_CreateRenderer(syslayer->window,
                                        -0x1,
                                        SDL_RENDERER_ACCELERATED);
  if (syslayer->render == null)
    {
      fprintf(stderr, "error, can't create render\n");
      return error_render;
    }

  syslayer->keys = new_arr(Bool, KEYS_LEN);
  memset(syslayer->keys, 0x0, sizeof(Bool)*KEYS_LEN);

  
  syslayer->is_render_inited = true;
  return ok;
}

Int
syslayer_terminate(None)
{
  if (syslayer == null || syslayer->is_render_inited == false)
    {
      fprintf(stderr, "warning, called " STR(__func__) " when\n"
              "system layer isn't inited\n");
      return warning_already_done;
    }
  SDL_DestroyRenderer(syslayer->render);
  SDL_DestroyWindow(syslayer->window);
  free(syslayer);
  syslayer = 0x0;
  SDL_Quit();
  return ok;
}

Int
game_init(None)
{
  gamesystem = new(GameSystem);
  if (gamesystem == null)
    {
      fprintf(stderr, "error, can't allocate memory for game\n");
      return error_out_of_mem;
    }
  return ok;
}

Int
game_terminate(None)
{
  if (gamesystem == null)
    {
      fprintf(stderr, "warning, tried to terminate non-inited game system\n");
      return warning_already_done;
    }
  free(gamesystem);
  return ok;
}

Int
game_user_terminate(None)
{
  gamesystem->is_running = false;
  game_terminate();
  syslayer_terminate();
  return ok;
}

Int
syslayer_get_events(None)
{
  SDL_Event event;
  while (SDL_PollEvent(&event))
    {
      switch(event.type)
        {
        case SDL_QUIT:
          game_user_terminate();
          break;
        case SDL_KEYDOWN:
          syslayer->keys[event.key.keysym.scancode] = true;
          break;
        case SDL_KEYUP:
          syslayer->keys[event.key.keysym.scancode] = false;
          break;
        }
    }
  return ok;
}

Int
syslayer_clear_window(GameColor color)
{
  if (syslayer == null || syslayer->is_render_inited == false)
    {
      fprintf(stderr, "warning, tried to clear non-intied window\n");
      return warning_uninited;
    }
  SDL_SetRenderDrawColor(syslayer->render,
                         color.r, color.g, color.b, color.a);
  SDL_RenderClear(syslayer->render);
  return ok;
}

Int
syslayer_draw_window(None)
{
  if (syslayer == null || syslayer->is_render_inited == false)
    {
      fprintf(stderr, "warning, tried to draw not-inited window\n");
      return warning_uninited;
    }
  SDL_RenderPresent(syslayer->render);
  return ok;
}

Int
syslayer_draw_rect(GameRect rect, GameColor color)
{
  SDL_SetRenderDrawColor(syslayer->render, color.r, color.g, color.b, color.a);
  SDL_FRect sdlrect = (SDL_FRect)
    {
      .x = rect.x,
      .y = rect.y,
      .w = rect.w,
      .h = rect.h
    };
  SDL_RenderFillRectF(syslayer->render, &sdlrect);
  return ok;
}

Int
game_process_events(None)
{
  syslayer_get_events();
  
  return ok;
}
