#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <uthread.h>
#include "private.h"

unsigned long fib(unsigned int n) {
    if (n == 0) return 0;
    if (n == 1) return 1;
    return fib(n - 1) + fib(n - 2);
}

void thread4(void *arg)
{
    (void)arg;
    for (int i = 0; i <= 40; i++) {
    	if (i == 35) {
    		preempt_stop();
    		printf("\nStopped Preemption.\n\n");
    	}
        printf("thread 4(%d): %ld\n", i, fib(i));
    }
    printf("thread4 complete\n");
}

void thread3(void *arg)
{
    (void)arg;
    uthread_create(thread4, NULL);
    for (int i = 0; i <= 40; i++) {
        printf("thread 3(%d): %ld\n", i, fib(i));
    }
    printf("thread3 complete.\n");
}

void thread2(void *arg)
{
    (void)arg;
    uthread_create(thread3, NULL);
    for (int i = 0; i <= 40; i++) {
        printf("thread 2(%d): %ld\n", i, fib(i));
    }
    printf("thread2 complete.\n");
}

void thread1(void *arg)
{
    (void)arg;
    uthread_create(thread2, NULL);
    for (int i = 0; i <= 40; i++) {
    	if (i == 35) {
    		preempt_start(true);
    		printf("\nStarted Preemption.\n\n");
    	}
    	printf("thread 1(%d): %ld\n", i, fib(i));
    }
    printf("thread1 complete.\n");
}

int main(void)
{
    uthread_run(false, thread1, NULL);
    return 0;
}
