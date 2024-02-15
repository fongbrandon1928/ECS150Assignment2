#include <stdbool.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

#include <uthread.h>

unsigned long fib(unsigned int n) {
    if (n == 0) return 0;
    if (n == 1) return 1;
    return fib(n - 1) + fib(n - 2);
}

void thread3(void *arg)
{
    (void)arg;
    for (int i = 0; i < 1000000; i++) {
        printf("thread3: %d\n", i);
    }
    printf("thread3\n");
}

void thread2(void *arg)
{
    (void)arg;
    uthread_create(thread3, NULL);
    for (int i = 0; i < 1000000; i++) {
        printf("thread2: %d\n", i);
    }
    printf("thread2\n");
}

void thread1(void *arg)
{
    (void)arg;
    uthread_create(thread2, NULL);
    for (int i = 0; i < 1000000; i++) {
    	printf("thread1: %d\n", i);
    }
    printf("thread1\n");
}

int main(void)
{
    uthread_run(true, thread1, NULL);
    return 0;
}
