//
//Deadlock originating in thread_mutex_lock
//

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

//CS520 threads
#include "thread.h"

thread_mutex_t mu;
thread_cond_t cv;

//local function prototypes
void error(char *);
void work(void *);
void work2(void * in);

int main(int argc, char *argv[])
{
  //Initialize mutex and condvar
  printf("Initializing variables...\n");
  thread_cond_init(&cv);
  thread_mutex_init(&mu);
  
  //Create threads
  printf("Parent creating thread 1...\n");
  thread_create(work2, (void*) 1);
  printf("Parent creating thread 2...\n");
  thread_create(work, (void*) 2);
  printf("Parent creating thread 3...\n");
  thread_create(work, (void*) 3);
  
  //Parent locks mutex
  //printf("Parent locking mutex...\n");
  
  
  //Parent yields
  printf("Parent yielding...\n");
  thread_yield();
  
  //Parent locks
  printf("Parent tries to lock...\n");
  thread_mutex_lock(&mu);
  
  fprintf(stderr, "Everything succeeded but deadlock expected!\n");
  
  return 0;
}

void work(void * in)
{
  long id = (long) in;
  
  printf("Child %ld tries to lock...\n", id);
  thread_mutex_lock(&mu);
  printf("Child %ld is unlocking...\n", id);
  thread_mutex_unlock(&mu);
  return;
}

void work2(void * in)
{
  long id = (long) in;
  
  printf("child %ld locks mutex...\n", id);
  thread_mutex_lock(&mu);
  printf("child %ld leaves...\n", id);
  return;
}

void error(char * str)
{
  fprintf(stderr, "%s\n", str);
  exit(-1);
}