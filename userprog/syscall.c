#include "userprog/syscall.h"
#include <stdio.h>
#include <syscall-nr.h>
#include "threads/interrupt.h"
#include "threads/thread.h"

static void syscall_handler (struct intr_frame *);

void
syscall_init (void) 
{
  intr_register_int (0x30, 3, INTR_ON, syscall_handler, "syscall");
}

static void
syscall_handler (struct intr_frame *f UNUSED) 
{
   uint32_t* esp = f->esp;
  uint32_t syscall = *esp;
  esp++;
  switch(syscall) {
    case SYS_WRITE: {
      int fd = *esp; // Esto será importane despues OJITO es el apuntador al stack
      esp++;
      void* buffer = (void*)*esp;
      esp++;
      unsigned int size = *esp;
      
      putbuf(buffer, size);
      
      break;
    }
    case SYS_EXEC:{
      char* cmd = (char*) *esp; //Obtienes el programa a ejecutar
      int call_return = process_execute (cmd);
      f->eax = call_return;
      break;
    }
    case SYS_WAIT:{
      tid_t child = *esp;
      f->eax =  process_wait(child);
      break;
    }
    //wait agregar llamada al semaforo del padre
    case SYS_EXIT: {
      int status = *esp;
      struct thread *t = thread_current();
      t->exit_status = status;
      printf("%s: exit(%d)\n", t->name,status);
      //Aqui se le debe informar al padre con que valor salio su hijo
      thread_exit ();
      break;
    }
  }
}
