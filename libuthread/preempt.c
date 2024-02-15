#include <signal.h>
#include <stdbool.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "private.h"
#include "uthread.h"

/*
 * Frequency of preemption
 * 100Hz is 100 times per second
 */
#define HZ 100
#define secCnvrtMicro 1000000
#define wholeSec 0
struct sigaction saDef;
struct itimerval old; 
sigset_t sigset;

void alrm_handler(int signum){
	(void)signum;
	uthread_yield();
}

void preempt_disable(void)
{
    sigemptyset(&sigset);
    sigaddset(&sigset, SIGVTALRM);
    sigprocmask(SIG_BLOCK, &sigset, NULL);
}

void preempt_enable(void)
{
    sigemptyset(&sigset);
    sigaddset(&sigset, SIGVTALRM);
    sigprocmask(SIG_UNBLOCK, &sigset, NULL);
}

void preempt_start(bool preempt)
{
	printf("preempt started1");
	if (!preempt) {
		return;
	}
	printf("preempt started2");
	saDef.sa_handler = alrm_handler;
	sigemptyset(&saDef.sa_mask);
	saDef.sa_flags = 0;
	sigaction(SIGVTALRM, &saDef, NULL);
	printf("preempt started3");
	//make in such that it runs 100 times per second
	struct itimerval timer;
	timer.it_value.tv_sec = wholeSec;
	timer.it_value.tv_usec = secCnvrtMicro/HZ;
	printf("preempt started4");
	//put in a initial start to 100microsec/100
	timer.it_interval.tv_sec = wholeSec;
	timer.it_interval.tv_usec = secCnvrtMicro/HZ;
	printf("preempt started5");
	//initalize new Itimerval for initial signal time and for everyother signal after that
	setitimer(ITIMER_VIRTUAL, &timer, &old);
	printf("preempt started6");
}

void preempt_stop(void)
{
	setitimer(ITIMER_VIRTUAL, &old, NULL);
	sigaction(SIGVTALRM, &saDef, NULL);
}
