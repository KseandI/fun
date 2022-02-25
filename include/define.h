
#ifndef GAME_INCLUDE_DEFINE
#define GAME_INCLUDE_DEFINE

/* to string */
#define _STR(X) #X
#define STR(X) _STR(X)

/* allocate new */
#define new(tp) (tp*) malloc(sizeof(tp))

#endif /* GAME_INCLUDE_DEFINE */
