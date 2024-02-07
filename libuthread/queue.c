#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

typedef struct node {
	void *data;
	struct node *next;
} node_t;

struct queue {
	node_t *tail;
	node_t *head;
	int length;
	/* TODO Phase 1 */
};

queue_t queue_create(void)
{
	queue_t q = (queue_t)malloc(sizeof(struct queue));
	if (!q) {
		return NULL;
	}

	q->head = NULL;
	q->tail = NULL;
	q->length = 0;
	return q;
	/* TODO Phase 1 */
}

int queue_destroy(queue_t queue)
{
	if (!queue || queue->length != 0) {
		return -1;
	}
	else {
		free(queue);
		return 0;
	}
	/* TODO Phase 1 */
}

int queue_enqueue(queue_t queue, void *data)
{
	if (queue == NULL || data == NULL) {
		return -1;
	}

	node_t *new_node = (node_t *)malloc(sizeof(node_t));
	if (!new_node) {
		return -1;
	}

	new_node->data = data;
    new_node->next = NULL;

	if (queue->tail == NULL) {
		// Check if queue is empty. Data set to both head and tail.
        queue->head = new_node;
        queue->tail = new_node;
    } else {
        // Add the new node at the end of the queue and update tail
        queue->tail->next = new_node;
        queue->tail = new_node;
    }

    // Increment the length of the queue
    queue->length++;

    return 0;
	/* TODO Phase 1 */
}

int queue_dequeue(queue_t queue, void **data)
{
	/* TODO Phase 1 */
}

int queue_delete(queue_t queue, void *data)
{
	/* TODO Phase 1 */
}

int queue_iterate(queue_t queue, queue_func_t func)
{
	/* TODO Phase 1 */
}

int queue_length(queue_t queue)
{
	if (queue == NULL) {
		return -1;
	}
	return queue->length;
	/* TODO Phase 1 */
}

