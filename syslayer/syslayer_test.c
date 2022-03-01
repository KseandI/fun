
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

  if (result < ok)
    {
      fprintf(stderr,
              " ==> error on syslayer init test, returned error\n"
              " ==>> error code: %d\n", result);
      return fail;
    }
  if (result > ok)
    {
      fprintf(stderr,
              " ==> error on syslayer init test, returned warn\n"
              " ==>> error code: %d\n", result);
      return fail;
    }
  if (syslayer == null)
    {
      fprintf(stderr,
              " ==> error on syslayer init test, returned ok, "
              "but syslayer doesn't inited\n");
      return fail;
    }
  fprintf(stderr, " ==> no errors on syslayer init test\n");
  return ok;
}

Test
test_syslayer_terminate(None)
{
  Int result;

  if (syslayer == null)
    syslayer_init();
  if (syslayer == null)
    {
      fprintf(stderr, " ==> error on syslayer termination test, can't init it\n");
      return fail;
    }

  result = syslayer_terminate();

  if (result < ok)
    {
      fprintf(stderr, " ==> error on syslayer termination test, returned error\n"
              " ==>> error code: %d\n", result);
      return fail;
    }
  if (result > ok)
    {
      fprintf(stderr, " ==> error on syslayer termination test, returned warn\n"
              " ==>> warning code: %d\n", result);
      return fail;
    }
  if (syslayer != null)
    {
      fprintf(stderr, " ==> error on syslayer termination test, function doens't "
              "cleared global variable\n");
      return fail;
    }
  
  fprintf(stderr, " ==> no errors on syslayer termination test\n");

  return ok;
}

Test
test_syslayer(None)
{
  fprintf(stdout, " => testing syslayer\n");
  CVOTE_TEST_BEGIN();
  CVOTE_TEST(test_syslayer_init);
  CVOTE_TEST(test_syslayer_terminate);
  CVOTE_TEST_END();
  return ok;
}
