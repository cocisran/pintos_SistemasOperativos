

#include "threads/thread.h"
#include <debug.h>
#include <list.h>

/*Este archivo maneja la reservacion de paginas virtuales para los procesos de usuario*/

struct frame_entry
{
    struct thread *t;  /*thread al cual esta asociado el marco*/
    uint8_t* upage;   /*Pagina virtual */
    uint8_t* kpage;   /*Marco que aloja la pagina*/
    struct list_elem elem;  /*Para poder construir una lista*/
};


/*Funcion que inicializa la lista de marcos*/
void frame_init(void);

/*Funcion que permite alojar una pagina dentro de un marco*/
void*  frame_get_page(uint8_t *);

/*Agrega una entrada a la tabla de paginacion*/
void add_frame(uint8_t *, uint8_t *);

/*Libera un marco de la memoria principal mandandolo a almacenamiento
secundario*/
void   free_space(void);