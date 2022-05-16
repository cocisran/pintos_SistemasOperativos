/*Este archivo sirve para mantener el nuevo mapeo de la memoria
virtual de pintos*/
#include "vm/frame.h"

#include <stdio.h>
#include <stdlib.h>
#include "threads/palloc.h"
#include "threads/thread.h"
#include "threads/synch.h"
#include "threads/malloc.h"
#include "userprog/pagedir.h"

static struct list frames_list; /*Lista de los marcos de cada proceso*/
static struct lock frame_list_lock; /*Semaforo para limitar la edicion de la lista de frames*/

static struct frame_entry* choose_victim_frame(void);

/*Inicia la lista global de marcos*/
void 
frame_init(void)
{
    list_init(&frames_list);
    lock_init(&frame_list_lock);
}


/*Aloja una pagina de proceso dentro de un margo*/
void* 
frame_get_page(uint8_t *upage)
{
    /*Obtener un marco para alojar la pagina*/
    uint8_t *kpage = palloc_get_page (PAL_USER);
    /*Si el no hay un marco disponible recurrir al swap*/

    /*Una vez que fue exitoso el alojamiento registrarlo en la lista*/
    add_frame(upage,kpage);
    return kpage;
}


/*Agrega la informacion administrativa del mapeo  a la memoria 
virtual de este proceso a la tabla de paginacion*/
void
add_frame(uint8_t *upage, uint8_t *kpage)
{
    lock_acquire (&frame_list_lock);
    struct  thread *t = thread_current();
    struct frame_entry* f = (struct frame_entry*)malloc(sizeof(struct frame_entry));
    f->t = t;
    f->upage = upage;
    f->kpage = kpage;
    list_push_back (&frames_list, &f->elem);
    lock_release (&frame_list_lock);
}


/*Libera espacio en la memoria principal realizando swaping.
Debe ser llamada desde un contexto safe thread, pues pueden haber errores
si es accesada de forma concurrente*/
void  
free_space()
{
/*Obtener pagina victima*/
    struct frame_entry* victim = choose_victim_frame();

    uint32_t *pd = victim->t->pagedir;
    uint8_t *upage = victim->upage;

/*Respaldar pagina en memoria secundaria*/

/*Libera la pagina*/
    pagedir_clear_page (pd,upage); /*pagedir libera el marco?*/
}

/*Funcion que escoge un marco victima para liberar a memoria secundaria*/
struct frame_entry* 
choose_victim_frame()
{
    return list_entry (list_pop_front (&frames_list), struct frame_entry, elem);
}