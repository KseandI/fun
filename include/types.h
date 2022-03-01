
#ifndef GAME_INCLUDE_TYPES
#define GAME_INCLUDE_TYPES

#include <stdint.h>

typedef int32_t Int;
typedef double Float;
typedef uint32_t UInt;
typedef int_fast8_t SInt;
typedef int_fast8_t Bool;
typedef float SFloat;
typedef void None;
typedef void* Ptr;

#define null NULL
#define true 0x1
#define false 0x0

#define error_test -0x4
#define error_system -0x3
#define error_upper -0x2
#define error_out_of_mem -0x1
#define ok 0x0
#define warn_init 0x1
#define warn_already 0x2
#define warn_uninited 0x3

typedef struct Vector2
{
  Float x, y;
} Vector2;

typedef struct GameColor
{
  SInt r, g, b, a;
} GameColor;

typedef struct GameRect
{
  Float x, y, w, h;
} GameRect;

#endif /* GAME_INCLUDE_TYPES */
