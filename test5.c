#include <stdio.h>
#include "thread.h"

#define status(part,total) fprintf(stderr, "(%d/%d)\n", part, total)

void thread1(void* info);
void thread2(void* info);

int main(void)
{
  thread_create(thread1, 0);
  status(0,5);
  thread_yield();
  status(2,5);
  thread_yield();
  status(5,5);
  return 0;
}

void thread1(void* info) 
{
  status(1,5);
  thread_create(thread2, 0);
  thread_yield();
  status(4,5);
}

void thread2(void* info) 
{ 
  status(3,5);
  thread_yield();
}
