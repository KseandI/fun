
#ifndef GAME_TESTING_INCLUDE
#define GAME_TESTING_INCLUDE

#include "standard.h"
#include "types.h"
#include "define.h"

#define fail -0x1
#define CVOTE_TEST_BEGIN() Test CVOTE_TEST_RESULT_VAR = ok
#define CVOTE_TEST(func) if (func() != ok) CVOTE_TEST_RESULT_VAR = fail
#define CVOTE_TEST_END() return CVOTE_TEST_RESULT_VAR

typedef Bool Test;

#endif /* GAME_TESTING_INCLUDE */
