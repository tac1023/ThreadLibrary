//
//Hit all parts of code successfully
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
void work2(void *);

int main(int argc, char *argv[])
{
  //Call thread_yield with no other threads.
  printf("Parent yield test...\n");
  thread_yield();
  thread_yield();
  thread_yield();
  
  printf("Parent initializing mutex and condvar...\n");
  thread_mutex_init(&mu);
  thread_cond_init(&cv);
  
  printf("Parent sending thread_cond_signal...\n");
  if(thread_cond_signal(&cv) == 0)
  {
    error("thread_cond_signal failed, should have passed!");
  }
  
  printf("Parent creating thread 1...\n");
  if(thread_create(work, (void*) 1) == 0)
  {
    error("Failure to make thread 1!");
  }
  
  thread_yield();
  
  printf("Back to parent...\n");
  
  printf("parent creating thread 2...\n");
  if(thread_create(work, (void*) 2) == 0)
  {
    error("Failure to make thread 2!");
  }
  
  printf("Parent locking mutex...\n");
  if(thread_mutex_lock(&mu) == 0)
  {
    error("Parent failed to lock mutex!");
  }
  
  printf("Parent yielding...\n");
  thread_yield();
  
  printf("Parent unlocking mutex...\n");
  if(thread_mutex_unlock(&mu) == 0)
  {
    error("Parent failed to unlock mutex!");
  }
  printf("Parent yielding...\n");
  thread_yield();
  
  printf("Parent creating thread 3...\n");
  if(thread_create(work, (void*) 3) == 0)
  {
    error("Failure to make thread 3!");
  }
  printf("Parent creating thread 4...\n");
  if(thread_create(work, (void*) 4) == 0)
  {
    error("Failure to make thread 4!");
  }
  printf("Parent creating thread 5...\n");
  if(thread_create(work, (void*) 5) == 0)
  {
    error("Failure to make thread 5!");
  }
  
  printf("Parent locking...\n");
  if(thread_mutex_lock(&mu) == 0)
  {
    error("Parent failed to lock mutex!");
  }
  
  printf("Parent yielding...\n");
  
  printf("Parent unlocking...\n");
  if(thread_mutex_unlock(&mu) == 0)
  {
    error("parent failed to unlock mutex!");
  }
  
  printf("Parent yielding...\n");
  thread_yield();
  
  printf("Parent creating thread 6...\n");
  if(thread_create(work2, (void*) 6) == 0)
  {
    error("Failure to make thread 6!");
  }
  
  printf("Parent yielding...\n");
  thread_yield();
  
  printf("Parent signaling...\n");
  if(thread_cond_signal(&cv) == 0)
  {
    error("Parent signaling failure!");
  }
  
  printf("Parent yielding...\n");
  thread_yield();
  
  
  printf("Parent creating thread 7...\n");
  if(thread_create(work2, (void*) 7) == 0)
  {
    error("Failure to make thread 7!");
  }
  printf("Parent creating thread 8...\n");
  if(thread_create(work2, (void*) 8) == 0)
  {
    error("Failure to make thread 8!");
  }
  printf("Parent creating thread 9...\n");
  if(thread_create(work2, (void*) 9) == 0)
  {
    error("Failure to make thread 9!");
  }
  
  printf("Parent yielding...\n");
  thread_yield();
  
  printf("Parent signaling...\n");
  if(thread_cond_signal(&cv) == 0)
  {
    error("Parent signaling failure!");
  }
  printf("Parent signaling...\n");
  if(thread_cond_signal(&cv) == 0)
  {
    error("Parent signaling failure!");
  }
  printf("Parent signaling...\n");
  if(thread_cond_signal(&cv) == 0)
  {
    error("Parent signaling failure!");
  }
  
  printf("Parent yielding...\n");
  thread_yield();
  printf("Parent yielding...\n");
  thread_yield();
  printf("Parent yielding...\n");
  thread_yield();
  
  printf("Parent creating thread 10...\n");
  if(thread_create(work2, (void*) 10) == 0)
  {
    error("Failure to make thread 10!");
  }
  
  printf("Parent yielding...\n");
  thread_yield();
  
  printf("Parent locking...\n");
  if(thread_mutex_lock(&mu) == 0)
  {
    error("Parent failed to lock mutex!");
  }
  
  printf("Parent signaling...\n");
  if(thread_cond_signal(&cv) == 0)
  {
    error("Parent signaling failure!");
  }
  
  printf("Parent unlocking...\n");
  if(thread_mutex_unlock(&mu) == 0)
  {
    error("parent failed to unlock mutex!");
  }
  
  printf("Parent yielding...\n");
  thread_yield();
  
  printf("Parent creating thread 11...\n");
  if(thread_create(work2, (void*) 11) == 0)
  {
    error("Failure to make thread 11!");
  }
  printf("Parent creating thread 12...\n");
  if(thread_create(work2, (void*) 12) == 0)
  {
    error("Failure to make thread 12!");
  }
  printf("Parent creating thread 13...\n");
  if(thread_create(work2, (void*) 13) == 0)
  {
    error("Failure to make thread 13!");
  }
  
  printf("Parent yielding...\n");
  thread_yield();
  
  printf("Parent locking...\n");
  if(thread_mutex_lock(&mu) == 0)
  {
    error("Parent failed to lock mutex!");
  }
  
  printf("Parent signaling...\n");
  if(thread_cond_signal(&cv) == 0)
  {
    error("Parent signaling failure!");
  }
  printf("Parent signaling...\n");
  if(thread_cond_signal(&cv) == 0)
  {
    error("Parent signaling failure!");
  }
  printf("Parent signaling...\n");
  if(thread_cond_signal(&cv) == 0)
  {
    error("Parent signaling failure!");
  }
  
  printf("Parent unlocking...\n");
  if(thread_mutex_unlock(&mu) == 0)
  {
    error("parent failed to unlock mutex!");
  }
  
  printf("Parent yielding...\n");
  thread_yield();
  printf("Parent yielding...\n");
  thread_yield();
  printf("Parent yielding...\n");
  thread_yield();
  
  printf("Done\n");
  
  return 0;
}

void work(void * in)
{
  long id = (long) in;
  
  printf("Child %ld lock mutex...\n", id);
  if(thread_mutex_lock(&mu) == 0)
  {
    error("Child unable to lock mutex!");
  }
  
  printf("Child %ld unlock mutex...\n", id);
  if(thread_mutex_unlock(&mu) == 0)
  {
    error("Child unable to unlock mutex!");
  }
  
  return;
}

void work2(void * in)
{
  long id = (long) in;
  
  printf("Child %ld lock mutex...\n", id);
  if(thread_mutex_lock(&mu) == 0)
  {
    error("Child unable to lock mutex!");
  }
  
  printf("Child %ld wait...\n", id);
  if(thread_cond_wait(&cv, &mu) == 0)
  {
    error("Child failure in wait!");
  }
  
  printf("Child %ld unlock mutex...\n", id);
  if(thread_mutex_unlock(&mu) == 0)
  {
    error("Child unable to unlock mutex!");
  }
  
  return;
}

void error(char * str)
{
  fprintf(stderr, "%s\n", str);
  exit(-1);
}