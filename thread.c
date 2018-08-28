/*
 * thread.c - C portion of the thread library implementation.
 *
 *          This just provides a stub for the visible functions!
 */

//-------Import Statements--------------------
#include "thread.h"
#include <stdlib.h>
#include <stdint.h>
#include <stdio.h>


//---------Static Prototypes------------------
static void rotateList(void);
static void startRdyList(void);
static void addToList(void);
static void unlockMutex(thread_mutex_t *mutex);
static void addToMutexQueue(thread_mutex_t *mutex);
static void addToRdyListFromMutexQueue(thread_mutex_t *mutex);
static void addToWaitQueue(thread_cond_t *condvar);
static void addToRdyListFromWaitQueue(thread_cond_t *condvar);
static void addToMutexQueueFromWaitQueue(thread_cond_t *condvar);
static void deadlock();

//---------Ready List Pointers-----------------
static TCB *head = NULL;
static TCB *tail = NULL;


//-----Static Global Variables--------------------
static uint64_t *lastStack = NULL;
static int rdyListSize = 0;
static unsigned int mallocSize = 65536;
static unsigned int thirdToLastQuadword = 8189;
static unsigned int secondToLastQuadword = 8190;


//-----Public Functions---------------------------

//Create a new thread
long thread_create(void (*func)(void*), void*info)
{
  //if the ready list is empty, add a TCB for main 
  if(rdyListSize == 0)
  {
    startRdyList();
    if(head == NULL) //if malloc failed, return NULL
    {
      return (long)NULL;
    }
  }
  
  //add a new TCB to the ready list
  addToList();
  if(tail == NULL) //if malloc failed, return NULL
  {
    return (long)NULL;
  }
  
  //malloc a new stack for the new thread
  tail->rbp = malloc(mallocSize);
  if(tail->rbp == NULL) //if malloc failed, return NULL
  {
    return (long)NULL;
  }
  
  //insert work function and parameter into rdi and rsi
  tail->rdi = (uint64_t)func;
  tail->rsi = (uint64_t)info;
  
  /*if(tail->rdi == 0) //DUBUG
  {
    fprintf(stderr, "rdi is NULL\n");
  }*/
  //fprintf(stderr, "%p\n", (tail->rbp + thirdToLastQuadword)); //DEBUG
  
  //put third to last quadword in rsp
  tail->rsp = (uint64_t)(tail->rbp + thirdToLastQuadword);
  
  //fprintf(stderr, "%p\n", thread_start); //DEBUG
  //fprintf(stderr, "%p\n", (void*)thread_start); //DEBUG
  //fprintf(stderr, "%p\n", (void*)thread_start); //DEBUG
  //tail->rbp[8] = (uint64_t*)thread_start; //DEBUG
  
  //put address of thread_start in second to last quadword
  tail->rbp[secondToLastQuadword] = (uint64_t)(void*)thread_start;
  
  return (long)tail;
}


//Give up CPU to the next thread
void thread_yield(void)
{
  //fprintf(stderr, "Entered thread_yield\n"); //DEBUG
  
  //if ready list is empty, add TCB for main
  if(rdyListSize == 0)
  {
    startRdyList();
  }
  
  //if there is more than 1 active thread, switch threads
  if(rdyListSize > 1)
  {
    rotateList();
    asm_yield(tail, head);
  }
}


//Initialize a given mutex: make sure data members are
//all null and start readyList if necessary
int thread_mutex_init(thread_mutex_t *mutex)
{
  //NULL mutex given, error
  if(mutex == NULL)
  {
    return 0;
  }
  
  //Start readyList if it is empty
  if(rdyListSize == 0)
  {
    startRdyList();
    if(head == NULL) //if malloc failed, return NULL
    {
      return 0;
    }
  }
  
  //Ensure all members are initially NULL
  mutex->owner = NULL;
  mutex->head = NULL;
  mutex->tail = mutex->head;
  
  return 1;
}


//Lock the given mutex from the current thread. If unlocked, 
//Give mutex to current thread. If locked, put current thread
//on mutex wait queue.
int thread_mutex_lock(thread_mutex_t *mutex)
{
  //NULL mutex given, error
  if(mutex == NULL)
  {
    return 0;
  }
  
  //No one is waiting, calling thread can take lock
  if(mutex->owner == NULL)
  {
    mutex->owner = head;
  }
  else //Others are waiting. Serve them first, have this one wait
  {
    //Calling thread already owns lock, error
    if(mutex->owner == head)
    {
      return 0;
    }
    else //queue current thread to wait, cycle ready list. 
    {
      addToMutexQueue(mutex); //Call helper function
      asm_yield(mutex->tail, head);
    }
  }
  
  return 1;
}


//Unlock the mutex and give it to the next thread on 
//the wait queue if there is one. 
int thread_mutex_unlock(thread_mutex_t *mutex)
{
  //NULL mutex given, error
  if(mutex == NULL)
  {
    return 0;
  }
  
  //mutex has no owner, isn't locked, error
  if(mutex->owner == NULL)
  {
    return 0;
  }
  
  //Calling thread isn't owner, error
  if(mutex->owner != head)
  {
    return 0;
  }
  
  unlockMutex(mutex);
  
  return 1;
}


//Initialize a given condition variable: make sure data members 
//are null and start readyList if necessary
int thread_cond_init(thread_cond_t *condvar)
{
  //NULL condition variable, error
  if(condvar == NULL)
  {
    return 0;
  }
  
  //Start readyList if it is empty
  if(rdyListSize == 0)
  {
    startRdyList();
    if(head == NULL) //if malloc failed, return NULL
    {
      return 0;
    }
  }
  
  //Ensure all members are initially NULL
  condvar->head = NULL;
  condvar->tail = NULL;
  
  return 1;
}


//Take the current thread off of the readyList, save a reference
//to its current mutex, unlock its mutex, and yield.
int thread_cond_wait(thread_cond_t *condvar, thread_mutex_t *mutex)
{
  //NULL condition variable or mutex, error
  if((condvar == NULL) || (mutex == NULL))
  {
    return 0;
  }
  
  //mutex has no owner, isn't locked, error
  if(mutex->owner == NULL)
  {
    return 0;
  }
  
  //Calling thread doesn't own mutex, error
  if(mutex->owner != head)
  {
    return 0;
  }
  
  //Remove thread from ready list, put on wait queue, save mutex
  //reference, unlock mutex
  addToWaitQueue(condvar);
  condvar->tail->myMutex = mutex;
  unlockMutex(mutex);
  asm_yield(condvar->tail, head);
  
  return 1;
}


//Wake up thread in wait queue (if applicable) checking its
//save mutex: if the mutex isn't locked, give the thread 
//the mutex and put it on the end of the ready list. If the
//mutex is locked, put the thread at the end of the mutex
//queue. 
int thread_cond_signal(thread_cond_t *condvar)
{
  //NULL condition variable, error
  if(condvar == NULL)
  {
    return 0;
  }
  
  //If no threads are waiting, just return successfully
  if((condvar->head == NULL) && (condvar->tail == NULL))
  {
    return 1;
  }
  
  //if mutex is unlocked, put thread directly on readyList
  if(condvar->head->myMutex->owner == NULL)
  {
    addToRdyListFromWaitQueue(condvar);
  }
  else //else, put thread into mutex queue
  {
    addToMutexQueueFromWaitQueue(condvar);
  }
  
  return 1;
}


//Start a thread if it hasn't been started before
void thread_start(void (*work)(void *), void *info)
{
  //fprintf(stderr, "Entered thread_start\n"); //DEBUG
  work(info);
  thread_cleanup();
}


//Deactivate and deallocate a thread
void thread_cleanup()
{
  //fprintf(stderr, "Entered thread_cleanup\n"); //DEBUG
  
  //remove front TCB from ready list
  struct TCB *rm = head;
  head = head->next;
  rm->next = NULL;
  rdyListSize--;
  
  //RdyListSize down to 0, deadlock
  if(rdyListSize == 0)
  {
    deadlock();
  }
  
  //free last stack (if there was one), get ready to free this stack
  if(lastStack != NULL)
  {
    free(lastStack);
  }
  lastStack = rm->rbp;
  
  //Free this TCB
  free(rm);
  
  //yield to next thread
  asm_yield(NULL, head);
}


//----------------Static Functions -----------------------------

//helper function for thread_yield.
//This rotates the ready list
static void rotateList(void)
{
  tail->next = head;
  tail = tail->next;
  head = tail->next;
  tail->next = NULL;
}


//function that initializes ready list
static void startRdyList(void)
{
  head = malloc(sizeof(struct TCB));
  tail = head;
  rdyListSize++;
}


//function for adding new TCB to ready list
static void addToList(void)
{
  tail->next = malloc(sizeof(struct TCB));
  tail = tail->next;
  rdyListSize++;
}


//Helper function, unlock a mutex
static void unlockMutex(thread_mutex_t *mutex)
{
  mutex->owner = NULL;
  
  //If someone is waiting, give them the mutex
  if((mutex->head != NULL) &&  (mutex->tail != NULL))
  {
    mutex->owner = mutex->head;
    addToRdyListFromMutexQueue(mutex);
  }
}


//Helper function, removes current thread from the ready list,
//Adds it to the mutex queue
static void addToMutexQueue(thread_mutex_t *mutex)
{
  //Adding to an empty mutex queue
  if((mutex->head == NULL) && (mutex->tail == NULL))
  {
    mutex->head = head;
    mutex->tail = mutex->head;
  }
  else //Adding to a non-empty mutex queue
  {
    mutex->tail->next = head;
    mutex->tail = mutex->tail->next;
  }
  
  head = head->next;
  mutex->tail->next = NULL; //get rid of reference to inside readyList
  rdyListSize--;
  
  //If readyList is empty, deadlock has occurred
  if(rdyListSize == 0)
  {
    deadlock();
  }
}


//Add the head of the mutex queue to the tail of the ready list.
static void addToRdyListFromMutexQueue(thread_mutex_t *mutex)
{
  tail->next = mutex->head;
  mutex->head = mutex->head->next;
  
  //If queue is now empty, set head/tail pointers accordingly
  if(mutex->head == NULL)
  {
    mutex->tail = mutex->head;
  }
  
  tail = tail->next;
  tail->next = NULL; //get rid of reference to inside mutex queue 
  rdyListSize++;
}


//Helper function, removes current thread from the ready list, 
//adds it to wait queue.
static void addToWaitQueue(thread_cond_t *condvar)
{
  //Adding to an empty wait queue
  if((condvar->head == NULL) && (condvar->tail == NULL))
  {
    condvar->head = head;
    condvar->tail = condvar->head;
  }
  else //Adding to a non-empty wait queue
  {
    condvar->tail->next = head;
    condvar->tail = condvar->tail->next;
  }
  
  head = head->next;
  condvar->tail->next = NULL; //get rid of reference to inside readyList
  rdyListSize--;
  
  //If readyList is empty, deadlock has occurred
  if(rdyListSize == 0)
  {
    deadlock();
  }
}


//Add the head of the wait queue to the tail of the readyList
static void addToRdyListFromWaitQueue(thread_cond_t *condvar)
{
  //set mutex owner to first thread in wait queue
  condvar->head->myMutex->owner = condvar->head;
  
  //add to readyList, remove from wait queue
  tail->next = condvar->head;
  condvar->head = condvar->head->next;
  
  //If queue is now empty, set head/tail pointers accordingly
  if(condvar->head == NULL)
  {
    condvar->tail = condvar->head;
  }
  
  tail = tail->next;
  tail->next = NULL; //get rid of reference to inside wait queue
  rdyListSize++;
  
}


//Add the head of the wait queue to the tail of the mutex queue
static void addToMutexQueueFromWaitQueue(thread_cond_t *condvar)
{
  //declare local helper variable
  TCB *temp;
  
  //add to mutex queue, remove from wait queue SPLIT INTO EMPTY/NON-EMPTY
  if((condvar->head->myMutex->head == NULL) && 
  (condvar->head->myMutex->tail == NULL))
  {
    condvar->head->myMutex->head = condvar->head;
    condvar->head->myMutex->tail = condvar->head->myMutex->head;
  }
  else
  {
    condvar->head->myMutex->tail->next = condvar->head;
    condvar->head->myMutex->tail = condvar->head->myMutex->tail->next;
  }
  
  temp = condvar->head;
  condvar->head=condvar->head->next;
  
  //If queue is now empty, set head/tail pointers accordingly
  if(condvar->head == NULL)
  {
    condvar->tail = condvar->head;
  }
  
  temp->myMutex->tail->next = NULL; //get rid of wait queue ref
  
  temp = NULL; //deref local var before deletion
  free(temp);
}


//DEADLOCK error, print error message and exit program
static void deadlock()
{
  fprintf(stderr, "FATAL ERROR: DEADLOCK\n");
  exit(-1);
}