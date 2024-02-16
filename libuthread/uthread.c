#include <assert.h>
#include <signal.h>
#include <stddef.h>
#include <stdint.h>
#include <stdio.h>
#include <stdlib.h>
#include <sys/time.h>

#include "private.h"
#include "uthread.h"
#include "queue.h"

static struct uthread_tcb *current_thread = NULL;
static struct uthread_tcb *idle_thread = NULL;
static queue_t ready_queue;

struct uthread_tcb
{
    uthread_ctx_t context;
    void *stack;
    enum
    {
        UTHREAD_READY,
        UTHREAD_RUNNING,
        UTHREAD_BLOCKED,
        UTHREAD_DONE
    } state;
};

struct uthread_tcb *uthread_current(void)
{
    return current_thread;
}

void uthread_yield(void)
{
    struct uthread_tcb *prev_thread = current_thread; //Save current thread before switching.
    if (queue_dequeue(ready_queue, (void **)&current_thread) == 0) //Dequeue next thread that is ready to run from ready_queue.
    {
        current_thread->state = UTHREAD_RUNNING; //State -> RUNNING (now running on CPU)
        if (prev_thread->state != UTHREAD_DONE) //If previous thread still ready, it is enqueued back
        {
            prev_thread->state = UTHREAD_READY;
            queue_enqueue(ready_queue, prev_thread);
        }
        uthread_ctx_switch(&prev_thread->context, &current_thread->context);
        //Perform context switch between previous and next thread. Saves context.
    }
}

void uthread_exit(void)
{
    current_thread->state = UTHREAD_DONE;
    uthread_yield();
}

int uthread_create(uthread_func_t func, void *arg)
{
    struct uthread_tcb *new_thread = malloc(sizeof(struct uthread_tcb)); //Memory dynamically allocated for new TCB. Holds context.
    if (!new_thread) //Returns -1 if allocation fails.
        return -1;

    new_thread->stack = uthread_ctx_alloc_stack(); //Memory allocated for new thread's stack.
    if (!new_thread->stack) //Checks for successful allocation.
    {
        free(new_thread);
        return -1;
    }

    if (uthread_ctx_init(&new_thread->context, new_thread->stack, func, arg) != 0)
    //Thread's execution context initialized. Sets up context to start execution at func with arg as arguments.
    {
        uthread_ctx_destroy_stack(new_thread->stack);
        free(new_thread);
        return -1;
    }

    new_thread->state = UTHREAD_READY; //State of new thread set to READY (thread scheduled for execution).
    queue_enqueue(ready_queue, new_thread); //Add new thread to ready_queue (FIFO queue)
    return 0;
}

int uthread_run(bool preempt, uthread_func_t func, void *arg)
{
    if (preempt) {
        preempt_start(preempt);
    }
    ready_queue = queue_create(); //Creates FIFO queue
    idle_thread = malloc(sizeof(struct uthread_tcb)); //Allocates memory for TCB and checks if successful.
    if (!idle_thread)
        return -1;

    idle_thread->state = UTHREAD_RUNNING;
    current_thread = idle_thread; // Initialize current_thread to idle_thread
    //idle_thread -> RUNNING, current_thread = idle_thread (idle thread is current context of execution)

    if (uthread_create(func, arg) != 0) //If thread creation fails, free allocated resources and return -1.
    {
        free(idle_thread);
        queue_destroy(ready_queue);
        return -1;
    }

    while (queue_length(ready_queue) > 0)
    // Continually yields control to other threads.
    // Continues as long there are threads in the ready queue.
    {
        uthread_yield();
    }

    // Cleanup
    queue_destroy(ready_queue); //After all threads complete execution.
    free(idle_thread);
    return 0;
}

void uthread_block(void)
{
    current_thread->state = UTHREAD_BLOCKED;
    uthread_yield();
}

void uthread_unblock(struct uthread_tcb *uthread)
{
    if (uthread->state == UTHREAD_BLOCKED)
    {
        // Change the thread's state to READY.
        uthread->state = UTHREAD_READY;
        // Enqueue the thread to the ready queue so it can be scheduled.
        queue_enqueue(ready_queue, uthread);
    }
}
