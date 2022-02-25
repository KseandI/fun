
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
      return warn_inited;
    }

  syslayer = new(Syslayer);
  if (syslayer == null)
    {
      fprintf(stderr, "error, can't allocate memory for syslayer\n");
      return error_out_of_mem;
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

  free(syslayer);
  return ok;
}
