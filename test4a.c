#include <stdio.h>
#include "thread.h"

#define status(part,total) fprintf(stderr, "(%d/%d)\n", part, total)

void thread1(void*);

int main(void)
{
  thread_create(thread1, 0);
  thread_yield();

  /* at this point, this is the only thread */
  thread_yield();
  fprintf(stderr," **Only running thread yielded and continued\n");
  status(1,1);
  return 0;
}

void thread1(void* fubar) 
{
  status(0,1);
  fprintf(stderr," ** Thread 1 running after main yielded\n");
  /* does nothing */
}
