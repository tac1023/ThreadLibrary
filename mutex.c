//
// Simple test for CS520 threads.
//   Tests queuing of threads waiting for mutex.
//
 
#include <stdio.h>
#include <stdlib.h>
#include <errno.h>
#include <unistd.h>

// CS520 threads
#include "thread.h"

thread_mutex_t mu;

// local function prototypes
void error(char *);
void work(void *);

int main (int argc, char *argv[])
{

  if (argc != 1) 
  {
    fprintf(stderr, "command-line arguments ignored!\n");
  }

  // initialize the mutex
  if (thread_mutex_init(&mu) == 0)
  {
    error("can't init mutex");
  }

  printf("parent locking...\n");
  if (thread_mutex_lock(&mu) == 0)
  {
    error("error in mutex_lock in parent thread");
  }

  printf("parent creating thread 1...\n");
  if (thread_create(work, (void *) 1) == 0)
  {
    error("error in thread create #1");
  }

  printf("parent yielding...\n");
  thread_yield();

  printf("parent creating thread 2...\n");
  if (thread_create(work, (void *) 2) == 0)
  {
    error("error in thread create #2");
  }

  printf("parent yielding...\n");
  thread_yield();

  printf("parent creating thread 3...\n");
  if (thread_create(work, (void *) 3) == 0)
  {
    error("error in thread create #3");
  }

  printf("parent yielding...\n");
  thread_yield();

  printf("parent unlocking...\n");
  if (thread_mutex_unlock(&mu) == 0)
  {
    error("error in mutex_unlock in parent thread");
  }

  printf("parent yielding...\n");
  thread_yield();

  printf("parent yielding...\n");
  thread_yield();

  printf("parent yielding...\n");
  thread_yield();

  return 0;
}

void work(void * in)
{
  // logical thread id (0..n-1)
  long id = (long) in;

  printf("child %ld locking...\n", id);
  if (thread_mutex_lock(&mu) == 0)
  {
    error("error in mutex_lock in child");
  }

  printf("child %ld unlocking...\n", id);
  if (thread_mutex_unlock(&mu) == 0)
  {
    error("error in mutex_unlock in child");
  }

  return;
}

void error(char *str)
{
  fprintf(stderr, "%s\n", str);
  exit(-1);
}

