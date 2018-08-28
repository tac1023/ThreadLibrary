//
//Test passing NULL into all mutex and condvar
//primitives. Lock a thread twice, unlock and unlocked
//thread, and call unlock when not owner. Call wait on
//a thread when it doesn't own the mutex.
//

#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

//CS520 threads
#include "thread.h"

thread_mutex_t mu;
thread_cond_t cv;

// local function prototypes
void error(char *);
void work(void *);


int main(int argc, char *argv[])
{
  if(argc != 1)
  {
    fprintf(stderr, "command-line arguments ignored!\n");
  }
  
  //Initialize mutex and condvar
  fprintf(stdout, "Initializing mutex and condvar correctly...\n");
  if(thread_mutex_init(&mu) == 0)
  {
    error("thread_mutex_init failed, should have passed!");
  }
  if(thread_cond_init(&cv) == 0)
  {
    error("thread_cond_init failed, should have passed!");
  }
  
  //Pass NULL to thread_mutex_init
  fprintf(stdout, "Passing NULL to thread_mutex_init...\n");
  if(thread_mutex_init(NULL) != 0)
  {
    error("thread_mutex_init passed, should have failed!");
  }
  
  //Pass NULL to thread_mutex_lock
  fprintf(stdout, "Passing NULL to thread_mutex_lock...\n");
  if(thread_mutex_lock(NULL) != 0)
  {
    error("thread_mutex_lock passed, should have failed!\n");
  }
  
  //Pass NULL to thread_mutex_unlock
  fprintf(stdout, "Passing NULL to thread_mutex_unlock...\n");
  if(thread_mutex_unlock(NULL) != 0)
  {
    error("thread_mutex_unlocked passed, should have failed!\n");
  }
  
  //Pass NULL to thread_cond_init
  fprintf(stdout, "Passing NULL to thread_cond_init...\n");
  if(thread_cond_init(NULL) != 0)
  {
    error("thread_cond_init passed, should have failed!\n");
  }
  
  //Pass 2 NULL to thread_cond_wait
  fprintf(stdout, "Passing 2 NULLs to thread_cond_wait...\n");
  if(thread_cond_wait(NULL, NULL) != 0)
  {
    error("thread_cond_wait passed, should have failed!\n");
  }
  
  //Pass NULL cond, non-NULL mutex to thread_cond_wait
  fprintf(stdout, "Passing NULL condvar and non-NULL mutex to thread_cond_wait...\n");
  if(thread_cond_wait(NULL, &mu) != 0)
  {
    error("thread_cond_wait passed, should have failed!");
  }
  
  //Pass non-NULL cond, NULL mutex to thread_cond_wait
  fprintf(stdout, "Passing non-NULL condvar and NULL mutex to thread_cond_wait...\n");
  if(thread_cond_wait(&cv, NULL) != 0)
  {
    error("thread_cond_wait passed, should have failed!");
  }
  
  //Pass NULL to thread_cond_signal
  fprintf(stdout, "Passing NULL to thread_cond_signal...\n");
  if(thread_cond_signal(NULL) != 0)
  {
    error("thread_cond_signal passed, should have failed!");
  }
  
  //Unlock not locked mutex
  fprintf(stdout, "Unlocking mutex that isn't locked...\n");
  if(thread_mutex_unlock(&mu) != 0)
  {
    error("thread_mutex_unlock passed, should have failed!");
  }
  
  //Wait on unlocked mutex
  fprintf(stdout, "Waiting with unlocked mutex...\n");
  if(thread_cond_wait(&cv, &mu) != 0)
  {
    error("thread_cond_wait passed, should have failed!\n");
  }
  
  //Lock mutex
  fprintf(stdout, "Locking mutex...\n");
  if(thread_mutex_lock(&mu) == 0)
  {
    error("thread_mutex_lock failed, should have passed!");
  }
  
  //Lock mutex again
  fprintf(stdout, "Locking mutex again...\n");
  if(thread_mutex_lock(&mu) != 0)
  {
    error("thread_mutex_lock passed, should have failed!");
  }
  
  //Create Thread 1
  fprintf(stdout, "Parent creating thread 1...\n");
  if(thread_create(work, (void *) 1) == 0)
  {
    error("Error in creating thread\n");
  }
  
  //Parent yield
  fprintf(stdout, "Parent yielding...\n");
  thread_yield();
  
  //Unlock parent
  fprintf(stdout, "Parent unlocking...\n");
  if(thread_mutex_unlock(&mu) == 0)
  {
    error("thread_mutex_unlock failed, should have passed!");
  }
  
  fprintf(stdout, "Done\n");
  return 0;
}

void work(void * in)
{
  // logical thread id
  long id = (long) in;
  
  //Unlock mutex that child doesn't own
  fprintf(stdout, "Child %ld unlocking parent's mutex...\n", id);
  if(thread_mutex_unlock(&mu) != 0)
  {
    error("thread_mutex_unlock passed, should have failed!");
  }
  
  //Call wait with mutex child doesn't own
  fprintf(stdout, "Child calling wait with parent's mutex...\n");
  if(thread_cond_wait(&cv, &mu) != 0)
  {
    error("thread_cond_wait passed, should have failed!");
  }
  
  return;
}

void error(char *str)
{
  fprintf(stderr, "%s\n", str);
  exit(-1);
}