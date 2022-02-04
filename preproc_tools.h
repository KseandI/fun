
#ifndef PREPROC_TOOLS_INC
#define PREPROC_TOOLS_INC

/* to string */
#define _STR(X) #X
#define STR(X) _STR(X)

/* terminal messages */
#define PRT_ERROR(...) \
  fprintf(stderr, ##__VA_ARGS__)
/**/
#ifdef PRINT_WARNING_MESSAGES
#define PRT_WARN(...) \
  fprintf(stderr, ##__VA_ARGS__)
#else
#define PRT_WARN(...)
#endif
/**/
#ifdef DEBUG_MODE
#define PRT_DEBUG(...) \
  fprintf(stderr, ##__VA_ARGS__)
#else
#define PRT_DEBUG(...)
#endif

#endif /* PREPROC_TOOLS_INC */

