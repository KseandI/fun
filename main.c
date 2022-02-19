
#include <stdio.h>
#include <stdlib.h>
#include <stdint.h>
#include <string.h>

#include <SDL2/SDL.h>

typedef int32_t Int;
typedef uint32_t UInt;
typedef double Float;
typedef float SFloat;
typedef int_fast8_t Bool;
typedef void None;
typedef void* Ptr;

#define null NULL
#define true 0x1
#define false 0x0

#define error_upper -0x3
#define error_out_of_mem -0x2
#define error_render -0x1
#define ok 0x0
#define warning_already_done 0x1

#define _STR(X) #X
#define STR(X) _STR(X)

#define new(X) (X*) malloc(sizeof(X))

typedef struct SystemLayer
{
  Bool is_render_inited;
} SystemLayer;

SystemLayer* syslayer;

Int
syslayer_init(None)
{
  syslayer = new(SystemLayer);
  if (syslayer == null)
    {
      fprintf(stderr, "error, can't allocate memory\b"
              "for system layer\n");
      return error_out_of_mem;
    }         
  memset(syslayer, 0x0, sizeof(SystemLayer));

  
  if (SDL_Init(SDL_INIT_VIDEO) < 0x0)
    {
      fprintf(stderr,
              "error, can't init sdl\n"
              "sdl log: %s\n", SDL_GetError());
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

  return syslayer_terminate();
}
