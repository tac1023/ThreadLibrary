#
# Makefile for thread tests
#

CC = gcc
CFLAGS = -g -Wall -std=c99
EXES = test1 test2 test3 test4a test4b test5 mutex condvar error1 error2 error3 pi2

all: $(EXES)

test1: test1.o thread.o thr_asm.o
	$(CC) $(CFLAGS) test1.o thread.o thr_asm.o -o test1

test2: test2.o thread.o thr_asm.o
	$(CC) $(CFLAGS) test2.o thread.o thr_asm.o -o test2

test3: test3.o thread.o thr_asm.o
	$(CC) $(CFLAGS) test3.o thread.o thr_asm.o -o test3

test4a: test4a.o thread.o thr_asm.o
	$(CC) $(CFLAGS) test4a.o thread.o thr_asm.o -o test4a

test4b: test4b.o thread.o thr_asm.o
	$(CC) $(CFLAGS) test4b.o thread.o thr_asm.o -o test4b

test5: test5.o thread.o thr_asm.o
	$(CC) $(CFLAGS) test5.o thread.o thr_asm.o -o test5

mutex: mutex.o thread.o thr_asm.o
	$(CC) $(CFLAGS) mutex.o thread.o thr_asm.o -o mutex

condvar: condvar.o thread.o thr_asm.o
	$(CC) $(CFLAGS) condvar.o thread.o thr_asm.o -o condvar

error1: error1.o thread.o thr_asm.o
	$(CC) $(CFLAGS) error1.o thread.o thr_asm.o -o error1

error2: error2.o thread.o thr_asm.o
	$(CC) $(CFLAGS) error2.o thread.o thr_asm.o -o error2
  
error3: error3.o thread.o thr_asm.o
  $(CC) $(CFLAGS) error3.o thread.o thr_asm.o -o error3

pi2: pi2.o thread.o thr_asm.o
	$(CC) $(CFLAGS) pi2.o thread.o thr_asm.o -o pi2

thread.o: thread.c
	$(CC) $(CFLAGS) -c thread.c

thr_asm.o: thr_asm.s
	$(CC) $(CFLAGS) -c thr_asm.s

clean:
	-rm -f *.o $(EXES)

