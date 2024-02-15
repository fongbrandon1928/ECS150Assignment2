#include <stddef.h>
#include <stdlib.h>

#include "queue.h"
#include "sem.h"
#include "private.h"

struct semaphore {
	size_t count;
	queue_t *waiting_threads;
} semaphore_t;

sem_t sem_create(size_t count)
{
	semaphore_t *sem = (semaphore_t *)malloc(sizeof(semaphore_t));
	if (sem == NULL) {
        // Allocation failed
        return NULL;
    }
	// Initialize the semaphore's count
    sem->count = count;

    // Initialize the waiting queue for threads
    sem->waiting_threads = queue_create();
    if (sem->waiting_threads== NULL) {
        // Queue allocation failed, clean up semaphore
        free(sem);
        return NULL;
    }
    return sem;
}

int sem_destroy(sem_t sem)
{
	if (sem == NULL) {
        // The semaphore is NULL, cannot destroy
        return -1;
    }
	semaphore_t *s = (semaphore_t *)sem;

	// Check if there are threads waiting on the semaphore
    if (queue_length(s->waiting_threads) > 0) {
        // There are still threads waiting on the semaphore, cannot destroy
        return -1;
    }
	// No threads are waiting, so we can safely destroy the semaphore

    // Destroy the waiting queue
    queue_destroy(s->wait_queue);

    // Deallocate the semaphore itself
    free(s);

    return 0;
}

int sem_down(sem_t sem)
{
	/* TODO Phase 3 */
}

int sem_up(sem_t sem)
{
	/* TODO Phase 3 */
}

