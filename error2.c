//
// Simple test for CS520 threads.
//   Tests error conditions for condition-variable functions.
//
 
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

// CS520 threads
#include "thread.h"

thread_mutex_t mu;
thread_cond_t cv;

void work(void *);

int main (int argc, char *argv[])
{

  if (argc != 1) 
  {
    fprintf(stderr, "command-line arguments ignored!\n");
  }

  if (thread_mutex_init(&mu) == 0)
  {
    fprintf(stderr, "call to thread_mutex_init fails that should succeed!\n");
  }

  if (thread_cond_init(&cv) == 0)
  {
    fprintf(stderr, "call to thread_cond_init fails that should succeed!\n");
  }

  if (thread_cond_wait(&cv, &mu) != 0)
  {
    fprintf(stderr, "call to thread_cond_wait succeeds that should fail!\n");
  }

  if (thread_mutex_lock(&mu) == 0)
  {
    fprintf(stderr, "call to thread_mutex_lock fails that should succeed!\n");
  }

  if (thread_create(work, (void *) 1) == 0)
  {
    fprintf(stderr, "call to thread_create fails that should succeed!\n");
  }

  thread_yield();

  if (thread_cond_init(0) != 0)
  {
    fprintf(stderr, "call to thread_cond_init succeeds that should fail!\n");
  }

  if (thread_cond_wait(0, &mu) != 0)
  {
    fprintf(stderr, "call to thread_cond_wait succeeds that should fail!\n");
  }

  if (thread_cond_wait(&cv, 0) != 0)
  {
    fprintf(stderr, "call to thread_cond_wait succeeds that should fail!\n");
  }

  if (thread_cond_signal(0) != 0)
  {
    fprintf(stderr, "call to thread_cond_signal succeeds that should fail!\n");
  }

  printf("Test complete.\n");
  return 0;
}

void work(void * in)
{
  if (thread_cond_wait(&cv, &mu) != 0)
  {
    fprintf(stderr, "call to thread_cond_signal succeeds that should fail!\n");
  }

  return;
}


