#include <stdio.h>
#include "thread.h"

#define status(part,total) fprintf(stderr, "(%d/%d)\n", part, total)

void thread1(void* info);

int main(void)
{
  thread_create(thread1, "info passed correctly");
  status(0,2);
  thread_yield();
  status(2,2);
  return 0;
}

void thread1(void* info) 
{
  status(1,2);
  fprintf(stderr, "%s\n", (char*)info);
  thread_yield();
  status(3,2); /* error if program gets here */
}
