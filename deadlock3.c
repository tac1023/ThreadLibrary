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
void work(void *);

int main(int args, char *argv[])
{
  //Initialize mutex and condvar
  printf("Initializing variables...\n");
  thread_mutex_init(&mu);
  thread_cond_init(&cv);
  
  //Create threads
  printf("Parent creating thread 1...\n");
  thread_create(work, (void*) 1);
  printf("Parent creating thread 2...\n");
  thread_create(work, (void*) 2);
  printf("Parent creating thread 3...\n");
  thread_create(work, (void*) 3);
  
  //Parent locks mutex
  printf("Parent locks mutex...\n");
  thread_mutex_lock(&mu);
  
  //Parent waits
  printf("Parent waits...\n");
  thread_cond_wait(&cv, &mu);
  
  //Parent yields
  printf("Parent yeilds...\n");
  thread_yield();
  
  fprintf(stderr, "Everything succeeded but deadlock expected!\n");
  return 0;
}

void work(void * in)
{
  long id = (long) in;
  
  //Lock mutex
  printf("Child %ld locks...\n", id);
  thread_mutex_lock(&mu);
  
  //Call wait
  printf("Child %ld waits...\n", id);
  thread_cond_wait(&cv, &mu);
  
  //Unlock mutex
  printf("Child %ld unlocks...\n", id);
  thread_mutex_unlock(&mu);
  
  return;
}

void error(char * str)
{
  fprintf(stderr, "%s\n", str);
  exit(-1);
}