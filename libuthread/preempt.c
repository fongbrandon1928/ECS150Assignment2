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
void alrm_handler(int signum){
	uthread_yield();
}

void preempt_disable(void)
{
    sigemptyset(&saDef);
    sigaddset(&saDef, SIGVTALRM);
    sigprocmask(SIG_BLOCK, &saDef, NULL);
}

void preempt_enable(void)
{
    sigemptyset(&saDef);
    sigaddset(&saDef, SIGVTALRM);
    sigprocmask(SIG_UNBLOCK, &saDef, NULL);
}

void preempt_start(bool preempt)
{

	if (!preempt) {
		return;
	}
	saDef.sa_handler = alrm_handler;
	sigemptyset(&saDef.sa_mask);
	saDef.sa_flags = 0;
	sigaction(SIGVTALRM, &saDef, NULL);

	//make in such that it runs 100 times per second
	struct itimerval timer;
	timer.it_value.tv_sec = wholeSec;
	timer.it_value.tv_usec = secCnvrtMicro/HZ;

	//put in a initial start to 100microsec/100
	timer.it_interval.tv_sec = wholeSec;
	timer.it_interval.tv_usec = secCnvrtMicro/HZ;

	//initalize new Itimerval for initial signal time and for everyother signal after that
	setitimer(ITIMER_VIRTUAL, &timer, &old);
}

void preempt_stop(void)
{
	setitimer(ITIMER_VIRTUAL, &old, NULL);
	sigaction(SIGVTALRM, &saDef, NULL);
}

