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

void alrm_handler(int signum)
{
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
	if (!preempt)
	{
		return;
	}

	saDef.sa_handler = alrm_handler;
	sigemptyset(&saDef.sa_mask);
	saDef.sa_flags = 0;
	sigaction(SIGVTALRM, &saDef, NULL);

	// Initialize timer to run 100 times per second.
	struct itimerval timer;
	timer.it_value.tv_sec = wholeSec;
	timer.it_value.tv_usec = secCnvrtMicro / HZ;

	// Set interval of repetitive signals to maintain 100HZ frequency.
	timer.it_interval.tv_sec = wholeSec;
	timer.it_interval.tv_usec = secCnvrtMicro / HZ;

	// Start the timer with ITIMER_VIRTUAL
	setitimer(ITIMER_VIRTUAL, &timer, &old);

}

void preempt_stop(void)
{
	setitimer(ITIMER_VIRTUAL, &old, NULL);
	sigaction(SIGVTALRM, &saDef, NULL);
}
