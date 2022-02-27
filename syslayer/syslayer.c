
#define SDL_DISABLE_IMMINTRIN_H
#include <SDL2/SDL.h>

#include "syslayer.h"

Syslayer* syslayer = null;

Int
syslayer_init(None)
{
  if (syslayer != null)
    {
      fprintf(stderr, "warning, syslayer already inited\n");
      return warn_init;
    }

  syslayer = new(Syslayer);
  if (syslayer == null)
    {
      fprintf(stderr, "error, can't allocate memory for syslayer\n");
      return error_out_of_mem;
    }

  syslayer->window = SDL_CreateWindow("fun",
                                      SDL_WINDOWPOS_UNDEFINED,
                                      SDL_WINDOWPOS_UNDEFINED,
                                      0x280, 0x1E0,
                                      SDL_WINDOW_RESIZABLE);
  if (syslayer->window == null)
    {
      fprintf(stderr, "error, can't create window\n");
      return error_system;
    }
  syslayer->render = SDL_CreateRenderer(syslayer->window, -0x1,
                                        SDL_RENDERER_ACCELERATED
                                        | SDL_RENDERER_PRESENTVSYNC);
  if (syslayer->render == null)
    {
      fprintf(stderr, "error, can't create render\n");
      return error_system;
    }
  
  return ok;
}

Int
syslayer_terminate(None)
{
  if (syslayer == null)
    {
      fprintf(stderr, "warn, tried to terminated uninitiallized syslayer\n");
      return warn_already;
    }

  SDL_DestroyRenderer(syslayer->render);
  SDL_DestroyWindow(syslayer->window);
  free(syslayer);
  return ok;
}

EventType
syslayer_read_event(Sysevent* event)
{
  SDL_Event sdl_event;
  SDL_PollEvent(&sdl_event);
  switch (sdl_event.type)
    {
    case SDL_QUIT:
      event->type = event_quit;
      return event->type;
      break;
    case SDL_KEYDOWN:
      event->type = event_keydown;
      event->key.code = sdl_event.key.keysym.scancode;
      return event->type;
      break;
    default:
      return event_none;
      break;
    }
  return event_none;
}

Int
syslayer_clear_window(GameColor color)
{
  if (syslayer == null || syslayer->render == null)
    {
      fprintf(stderr, "warn, tried to clearn uninitiallized window\n");
      return warn_uninited;
    }

  SDL_SetRenderDrawColor(syslayer->render,
                         color.r, color.g, color.b, color.a);
  SDL_RenderClear(syslayer->render);
  
  return ok;
}

Int
syslayer_draw_window(None)
{
  if (syslayer == null || syslayer->render == null)
    {
      fprintf(stderr, "warn, tried to draw uninitiallized window\n");
      return warn_uninited;
    }

  SDL_RenderPresent(syslayer->render);
  
  return ok;
}

Int
syslayer_draw_rect(GameRect rect, GameColor color)
{
  SDL_FRect sdlrect;
  if (syslayer == null || syslayer->render == null)
    {
      fprintf(stderr, "warn, drawed on uninited render\n");
      return warn_uninited;
    }
      
  SDL_SetRenderDrawColor(syslayer->render, color.r, color.g, color.b, color.a);
  
  sdlrect = (SDL_FRect)
    { .x = rect.x, .y = rect.y, .w = rect.w, .h = rect.h };
  SDL_RenderFillRectF(syslayer->render, &sdlrect);
  return ok;
}
