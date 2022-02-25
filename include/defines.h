
#ifndef GAME_DEFINES_INC
#define GAME_DEFINES_INC


/* everything to sting */
#define _STR(X) #X
#define STR(X) _STR(X)

/* allocate new type */
#define new(tp) (tp*) malloc(sizeof(tp))
#define new_arr(tp, sz) (tp*) malloc(sizeof(tp)*sz)


#endif /* GAME_DEFINES_INC */

