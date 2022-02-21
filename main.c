#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#define SDL_DISABLE_IMMINTRIN_H
#include <SDL2/SDL.h>

/* my types */
typedef int32_t Int;
typedef int8_t SInt;
typedef uint32_t UInt;
typedef double Float;
typedef float SFloat;
typedef int_fast8_t Bool;
typedef void None;
typedef void* Ptr;

#define null NULL
#define true 0x1
#define false 0x0

/* return codes table */
#define error_upper -0x3
#define error_out_of_mem -0x2
#define error_render -0x1
#define ok 0x0
#define warning_already_done 0x1
#define warning_uninited 0x2

/* everything to sting */
#define _STR(X) #X
#define STR(X) _STR(X)

/* allocate new type */
#define new(X) (X*) malloc(sizeof(X))

typedef struct SystemLayer
{
  Bool is_render_inited;
  Ptr window;
  Ptr render;
} SystemLayer;

SystemLayer* syslayer;

typedef struct GameSystem
{
  Bool is_running;
} GameSystem;

GameSystem* gamesystem;

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
        }
    }
  return ok;
}

Int
syslayer_clear_window(SInt r, SInt g, SInt b, SInt a)
{
  if (syslayer == null || syslayer->is_render_inited == false)
    {
      fprintf(stderr, "warning, tried to clear non-intied window\n");
      return warning_uninited;
    }
  SDL_SetRenderDrawColor(syslayer->render, r, g, b, a);
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
game_process_events(None)
{
  syslayer_get_events();
  return ok;
}

Int
main(None)
{
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

  gamesystem->is_running = true;

  while (gamesystem->is_running == true)
    {
      syslayer_clear_window(0, 0xff, 0xff, 0xff);
      syslayer_draw_window();
      game_process_events();
    }

  return ok;
}
