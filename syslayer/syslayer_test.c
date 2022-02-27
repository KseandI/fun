
#include "standard.h"
#include "types.h"
#include "define.h"

#include "syslayer.h"

#include "cvote_test.h"

Test
test_syslayer_init(None)
{
  Int result;

  if (syslayer != null)
    syslayer_terminate();

  result = syslayer_init();

  if (result != ok)
    {
      fprintf(stderr, "test error on syslayer init\n"
              "error code: %d\n", result);
      return fail;
    }
  else if (syslayer == null)
    {
      fprintf(stderr, "test error: on system initiallization no errors occurred\n"
              "but syslayer is still null\n");
      return fail;
    }
  else
    {
      fprintf(stderr, "syslayer inited successfully\n");
    }
  return ok;
}

Int
test_syslayer_terminate(None)
{
  Int result;

  if (syslayer == null)
    syslayer_init();
  if (syslayer == null)
    {
      fprintf(stderr, "error, can't init syslayer for termination test\n");
      return fail;
    }

  result = syslayer_terminate();

  if (result < ok)
    {
      fprintf(stderr, "error, can't terminate syslayer\n"
              "error code: %d\n", result);
      return fail;
    }
  else if (result > ok)
    {
      fprintf(stderr, "occurred some warning on system termination\n"
              "warning code: %d\n", result);
      return fail;
    }
  else
    {
      fprintf(stderr, "no error occurred on system terminate\n");
    }

  return ok;
}

