#include <stdio.h>
#include "thread.h"

#define status(part,total) fprintf(stderr, "(%d/%d)\n", part, total)

int main(void)
{
  status(0,1);
  thread_yield();
  status(1,1);
  return 0;
}
