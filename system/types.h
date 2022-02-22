
#ifndef GAME_TYPES_INC
#define GAME_TYPES_INC

#include "standard.h"

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

typedef struct SystemLayer
{
  Bool is_render_inited;
  Ptr window;
  Ptr render;
  Bool* keys;
} SystemLayer;

typedef struct GameSystem
{
  Bool is_running;
} GameSystem;

typedef struct GameColor
{
  SInt r, g, b, a;
} GameColor;

typedef struct GameRect
{
  Float x, y, w, h;
} GameRect;

typedef struct Vector2
{
  Float x, y;
} Vector2;


#endif /* GAME_TYPES_INC */
