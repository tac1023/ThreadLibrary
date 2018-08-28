//
//Deadlock originating in thread_cleanup
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
void work1(void *);
void work2(void *);

int main(int argc, char *argv[])
{
  //Initialize mutex and condvar
  printf("Initializing variables...\n");
  thread_mutex_init(&mu);
  thread_cond_init(&cv);
  
  //Create threads
  printf("Parent creating thread 1...\n");
  thread_create(work2, (void*) 1);
  printf("Parent creating thread 2...\n");
  thread_create(work1, (void*) 2);
  printf("Parent creating thread 3...\n");
  thread_create(work1, (void*) 3);
  
  //Yield to child 1
  printf("Parent is yielding...\n");
  thread_yield();
  
  //Lock parent
  printf("Parent tries to lock...\n");
  thread_mutex_lock(&mu);
  
  fprintf(stderr, "Everything succeeded but deadlock expected!");
  return 0;
}

void work1(void * in)
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
  
  printf("Child %ld is locking mutex...\n", id);
  thread_mutex_lock(&mu);
  thread_yield();
  printf("Child %ld is leaving now...\n", id);
  return;
}

void error(char * str)
{
  fprintf(stderr, "%s\n", str);
  exit(-1);
}