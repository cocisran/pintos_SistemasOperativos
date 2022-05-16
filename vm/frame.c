#include "vm/frame.h"

#include <stdio.h>

struct list frames; /*Lista de los marcos de cada proceso*/

/*Inicia la lista global de marcos*/
void 
frame_init(void)
{
    list_init(&frames);
}