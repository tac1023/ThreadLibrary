// interface for CS520 threads library
//
// Note: thread_mutex_t and thread_cond_t definitions are incomplete!
//

#include <stdlib.h>
#include <stdint.h>

long thread_create(void (*)(void*), void*);
void thread_yield(void);

typedef struct thread_mutex_t{ 
  struct TCB *owner;
  struct TCB *head;
  struct TCB *tail;
} thread_mutex_t;
int thread_mutex_init(thread_mutex_t *);
int thread_mutex_lock(thread_mutex_t *);
int thread_mutex_unlock(thread_mutex_t *);

typedef struct thread_cond_t{
  struct TCB *head;
  struct TCB *tail;
} thread_cond_t;
int thread_cond_init(thread_cond_t *);
int thread_cond_wait(thread_cond_t *, thread_mutex_t *);
int thread_cond_signal(thread_cond_t *);

//Struct for TCB
typedef struct TCB{
  uint64_t rsp;
  uint64_t rbx;
  uint64_t rdi;
  uint64_t rsi;
  uint64_t r12;
  uint64_t r13;
  uint64_t r14;
  uint64_t r15;
  struct TCB *next; //pointer to next TCB
  uint64_t *rbp; //pointer to base of stack
  struct thread_mutex_t *myMutex; //This thread's mutex at wait
} TCB;

void thread_start(void (*work)(void *), void *info);
void thread_cleanup();
void asm_yield(TCB *cur, TCB *next);