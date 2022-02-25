
#include "syslayer/syslayer.h"
#include "standard.h"
#include "types.h"
#include "define.h"

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
main(None)
{
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

  while (global != null && global->is_running == true)
    {
      /* code */
    }

  if (global != null) global_terminate();
  if (syslayer != null) syslayer_terminate();
  return ok;
}
