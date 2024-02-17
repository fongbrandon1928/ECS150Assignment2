#include <stddef.h>
#include <stdlib.h>

#include "queue.h"
#include "sem.h"
#include "private.h"

typedef struct semaphore
{
    size_t count;
    queue_t waiting_threads;
} semaphore_t;

semaphore_t *sem;

sem_t sem_create(size_t count)
{
    sem = (semaphore_t *)malloc(sizeof(semaphore_t));
    if (sem == NULL)
    {
        // Allocation failed
        return NULL;
    }
    // Initialize the semaphore's count
    sem->count = count;

    // Initialize the waiting queue for threads
    sem->waiting_threads = queue_create();
    if (sem->waiting_threads == NULL)
    {
        // Queue allocation failed, clean up semaphore
        free(sem);
        return NULL;
    }
    return sem;
}

int sem_destroy(sem_t sem)
{
    if (sem == NULL)
    {
        // The semaphore is NULL, cannot destroy
        return -1;
    }
    semaphore_t *s = (semaphore_t *)sem;

    // Check if there are threads waiting on the semaphore
    if (queue_length(s->waiting_threads) > 0)
    {
        // There are still threads waiting on the semaphore, cannot destroy
        return -1;
    }
    // No threads are waiting, so we can safely destroy the semaphore

    // Destroy the waiting queue
    queue_destroy(s->waiting_threads);

    // Deallocate semaphore
    free(s);

    return 0;
}

//thread acquires resource else blocked if not available
int sem_down(sem_t sem)
{
    if (!sem)
    {
        return -1;
    }

    while (sem->count == 0)
    {
        uthread_block();
    }
    sem->count--;
    return 0;
}

//indicate to a blocked thread of available resource
int sem_up(sem_t sem)
{
    if (!sem)
    {
        return -1;
    };
    sem->count++;
    void *data;
    int status = queue_dequeue(sem->waiting_threads, &data);
    if (status == -1)
    {
        return -1;
    }
    uthread_unblock((struct uthread_tcb *)data);
    return 0;
}
