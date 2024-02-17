
#include <stdio.h>
#include <assert.h>
#include <stdlib.h>
#include "queue.h"

void test_create(void)
{
    fprintf(stderr, "*** TEST create ***\n");

    assert(queue_create() != NULL);
}

/* Enqueue/Dequeue simple */
void test_queue_simple(void)
{
    int data = 3, *ptr;
    queue_t q;

    fprintf(stderr, "*** TEST queue_simple ***\n");

    q = queue_create();
    queue_enqueue(q, &data);
    queue_dequeue(q, (void**)&ptr);
    assert(ptr == &data);
}

//test successful deletion
void test_queue_delete(void)
{
    fprintf(stderr, "*** TEST queue_delete ***\n");
    int* ptr = malloc(sizeof(int));
    *ptr = 3;
    queue_t q = queue_create();
    queue_delete(q, (void*)ptr);
    assert(0 == queue_length(q));
}

//test enqueue mass operation of enqueue followed by delete
void test_crazy_enqueue(void)
{
    fprintf(stderr, "*** TEST queue_crazy_enqueue ***\n");
    queue_t q = queue_create();
    for(int i = 0; i < 1000000; i++)
    {
        queue_enqueue(q, &i);
    }
    queue_iterate(q, (queue_func_t) queue_delete);
    assert(queue_length(q) == 0);
}

//test successful dequeue
void test_dequeue(void)
{
    fprintf(stderr, "***TEST dequeue***\n");
    queue_t q = queue_create();
    for(int i = 0; i < 1000000; i++)
    {
        queue_enqueue(q, &i);
    }
    queue_iterate(q, (queue_func_t) queue_dequeue);
    assert(queue_length(q) == 0);
}

//test successful destroy
void test_destroy(void)
{
    fprintf(stderr, "***TEST destroy***\n");
    queue_t q = queue_create();
    for(int i = 0; i < 1000000; i++)
    {
        queue_enqueue(q, &i);
    }
    queue_iterate(q, (queue_func_t) queue_delete);
    int retrn = queue_destroy(q);
    assert(retrn == 0);
}

//test mass enqueue operations and length after each enqueue
void test_length(void)
{
    fprintf(stderr, "***TEST Length***\n");
    queue_t q = queue_create();
    void* data;
    for(int i = 0; i < 1000000; i++)
    {
        queue_enqueue(q, &i);
    }
    for(int j = 1000000; j >= 0; j--)
    {
        assert(queue_length(q) == j);
        queue_dequeue(q, &data);
    }
}

//test if contents of each node are correct
void test_contents()
{
    fprintf(stderr, "TEST Contents\n");
    queue_t q = queue_create();
    int *data; //Change to pointer type to store addresses
    int d;

    //Dynamically allocate memory for integers to ensure
    //they remain valid throughout the test.
    for(int i = 0; i < 1000000; i++)
    {
        int* ptr = malloc(sizeof(int));
        *ptr = i;
        queue_enqueue(q, ptr);
    }

    for(int j = 0; j < 1000000; j++)
    {
        queue_dequeue(q, (void**)&data); //cast to void** for dequeue
        d = *data; //Dereference pointer to get the integer value
        assert(d == j);
        free(data); //Free the dynamically allocated memory
    }

    //destroy queue
    queue_destroy(q);
}

//test if dequeue operation returns -1 on fail
void test_empty_dequeue()
{
    fprintf(stderr, "***TEST EMPTY DEQUEUE***\n");
    queue_t q = queue_create();
    void* data;
    assert(queue_dequeue(q, &data) == -1);
}

//test if enqueue operation returns -1 on fail
void test_enqueue_null()
{
    fprintf(stderr, "***TEST ENQUEUE NULL***\n");
    queue_t q = queue_create();
    assert(queue_enqueue(q, NULL) == -1);
}

//test attempt to destroy full queue
void test_full_queue_destroy()
{
    fprintf(stderr, "***TEST FULL QUEUE DESTROY***\n");
    queue_t q = queue_create();
    for(int i = 0; i < 5; i++)
    {
        queue_enqueue(q, &i);
    }
    assert(queue_destroy(q) == -1);
}



int main() {
    test_create();
    test_queue_simple();
    test_queue_delete();
    test_crazy_enqueue();
    test_dequeue();
    test_destroy();
    test_length();
    test_empty_dequeue();
    test_enqueue_null();
    test_full_queue_destroy();
    test_contents();
    return 0;
}
