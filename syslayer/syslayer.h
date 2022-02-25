
#ifndef GAME_SYSTEM_INCLUDE
#define GAME_SYSTEM_INCLUDE

#include "standard.h"
#include "types.h"
#include "define.h"

typedef struct Syslayer
{
  Ptr window;
  Ptr render;
} Syslayer;

extern Syslayer* syslayer;

Int syslayer_init(None);
Int syslayer_terminate(None);

#endif /* GAME_SYSTEM_INCLUDE */
