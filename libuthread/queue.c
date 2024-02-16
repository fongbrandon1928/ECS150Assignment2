#include <stdint.h>
#include <stdlib.h>
#include <string.h>

#include "queue.h"

typedef struct node
{
    void *data;
    struct node *next;
} node_t;

struct queue
{
    node_t *tail;
    node_t *head;
    int length;
};

queue_t queue_create(void)
{
    queue_t q = (queue_t)malloc(sizeof(struct queue));
    if (!q)
    {
        return NULL;
    }

    q->head = NULL;
    q->tail = NULL;
    q->length = 0;
    return q;
}

int queue_destroy(queue_t queue)
{
    if (!queue || queue->length != 0)
    {
        return -1;
    }
    else
    {
        free(queue);
        return 0;
    }
}

int queue_enqueue(queue_t queue, void *data)
{
    if (queue == NULL || data == NULL)
    {
        return -1;
    }

    node_t *new_node = (node_t *)malloc(sizeof(node_t));
    if (!new_node)
    {
        return -1;
    }

    new_node->data = data;
    new_node->next = NULL;

    if (queue->tail == NULL)
    {
        // Check if queue is empty. Data set to both head and tail.
        queue->head = new_node;
        queue->tail = new_node;
    }
    else
    {
        // Add the new node at the end of the queue and update tail
        queue->tail->next = new_node;
        queue->tail = new_node;
    }

    queue->length++;

    return 0;
}

int queue_dequeue(queue_t queue, void **data)
{
    if (queue == NULL || data == NULL || queue->length == 0)
    {
        return -1;
    }
    *data = queue->head->data;
    node_t *oldest_node = queue->head;
    if (queue->head->next == NULL)
    {
        queue->head = NULL;
        queue->tail = NULL;
    }
    else
    {
        queue->head = oldest_node->next;
    }
    free(oldest_node);
    queue->length--;
    return 0;
}

int queue_delete(queue_t queue, void *data)
{
    if (queue == NULL || data == NULL)
    {
        return -1;
    }

    node_t *current = queue->head;
    node_t *previous = NULL;

    while (current != NULL)
    {
        if (current->data == data)
        {
            // If the node to delete is the head
            if (previous == NULL)
            {
                queue->head = current->next;
                // If deleting the head makes the queue empty, also update the tail
                if (queue->head == NULL)
                {
                    queue->tail = NULL;
                }
            }
            else
            {
                // Update the previous node's next pointer
                previous->next = current->next;
                // If deleting the last node, update the tail
                if (current->next == NULL)
                {
                    queue->tail = previous;
                }
            }

            free(current);
            queue->length--;
            return 0;
        }

        previous = current;
        current = current->next;
    }

    return -1;
}

int queue_iterate(queue_t queue, queue_func_t func)
{
    if (queue == NULL || func == NULL)
    {
        return -1;
    }
    node_t *current = queue->head;
    node_t *next = NULL;

    // Iterate through the queue
    while (current != NULL)
    {
        // Save the next node before calling func, in case current node is deleted
        next = current->next;

        // Call the callback function on the current data
        func(queue, current->data);

        // Move to the next node
        current = next;
    }

    return 0;
}

int queue_length(queue_t queue)
{
    if (queue == NULL)
    {
        return -1;
    }
    return queue->length;
}
