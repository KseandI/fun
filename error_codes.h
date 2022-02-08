
#ifndef GAME_ERROR_INC
#define GAME_ERROR_INC


#define error_null -0x5     /*< passed argument is null */
#define error_upper -0x4    /*< recursive error */
#define error_system -0x3   /*< error on system(internal libs) side */
#define error_out_of_mem -0x2   /*< error on memory allocation */
#define error_lib -0x1          /*< error on some lib side */
#define error_none 0x0          /*< none error */
#define error_already_done 0x1  /*< operation already done */
#define error_data 0x2          /*< data doesn't exists */


#endif /* GAME_ERROR_INC */


