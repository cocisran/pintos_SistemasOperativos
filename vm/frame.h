

#include "threads/thread.h"
#include <debug.h>
#include <stdlib.h>
#include <list.h>

/*Este archivo maneja la reservacion de paginas virtuales para los procesos de usuario*/

struct frame_entry
{
    struct thread *t;  /*thread al cual esta asociado el marco*/
    uint8_t* upage;   /**/
    uint8_t* kpage;
    struct list_elem elem;  /*Para poder construir una lista*/
};


/*Funcion que inicializa la lista de marcos*/
void frame_init(void);
