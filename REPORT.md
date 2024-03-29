# Summary

The core of the project is to create a basic user-level thread library for 
Linux, providing interfaces for applications to spawn and manage threads 
running concurrently. This involves creating threads, scheduling them in a 
"first-come first-served" manner, preemptive scheduling via an interrupt-based 
scheduler, and offering a thread synchronization API through semaphores. 

## Makefile

This Makefile automates the compilation and creation of a static library named 
libuthread.a from source files queue.c, uthread.c, context.c, preempt.c, and 
sem.c using gcc for compilation and ar for archiving. The default all target 
compiles the sources into object files and then archives them into the static 
library. A clean target is provided to remove all generated files, ensuring a 
clean build environment. The Makefile uses .PHONY to mark all and clean as 
non-file targets, enforcing their execution regardless of file presence.

## Queue

queue.c implements a queue using a linked list, providing functions for 
creating, destroying, enqueuing, dequeuing, deleting specific elements, 
iterating over elements, and obtaining the queue's length. The queue is defined 
with head and tail pointers for efficient operations and a length attribute for 
tracking its size. The "queue_create" function initializes an empty queue. 
Elements can be added to the end of the queue with queue_enqueue and removed 
from the front with "queue_dequeue". The "queue_delete" function allows for the 
removal of a specific element. The "queue_iterate" function applies a callback 
to each element, and "queue_length" returns the current size of the queue. Error 
handling is included to manage null pointers and memory allocation failures, 
ensuring the queue's robust operation.

## Threads

uthread.c implements a user-level threading library, enabling concurrent 
execution through cooperative multitasking. It defines a thread control block 
(TCB) with a context, stack, and state. Key functions include "uthread_current" 
to get the current thread, "uthread_yield" to switch execution between threads 
while managing their states, and "uthread_exit" to mark a thread as done and 
yield control. "uthread_create" sets up new threads, allocating necessary 
resources and preparing them for execution by enqueuing them as READY. 
"uthread_run" initiates the threading system, creates an idle thread as the 
initial context, and enters a loop to yield to other threads until no more are 
ready, then cleans up. The library supports basic thread management operations 
like creating, yielding, exiting, and running threads, allowing for multiple 
tasks to run concurrently within a single process by switching contexts among 
user-level threads.

## Semaphore

sem.c defines a semaphore mechanism for synchronizing access to shared 
resources in a user-level threading environment. A semaphore includes a count 
to represent available resources and a queue for threads waiting to access 
these resources. The "sem_create" function initializes a semaphore with a given 
count and allocates a queue for waiting threads, handling potential allocation 
failures. "sem_destroy" deallocates the semaphore, ensuring no threads are 
waiting. The "sem_down" operation decrements the semaphore's count, blocking the 
thread if no resources are available, while "sem_up" increments the count and 
possibly unblocks a waiting thread, indicating a resource has been released. 
This implementation ensures safe concurrent access to resources, integrating 
with thread management through uthread_block and uthread_unblock functions to 
control thread states based on resource availability.

## Preemption

preempt.c implements preemption in a user-level threading system by using 
UNIX signals and timers, specifically employing SIGVTALRM and a virtual timer 
to periodically yield thread execution at a frequency of 100Hz. The 
"preempt_start" function sets up this mechanism, configuring a signal handler to 
call "uthread_yield", thus enforcing regular context switches among threads. It 
schedules the signal to occur every 10 milliseconds, ensuring equitable CPU 
time distribution. "preempt_stop" deactivates preemption by restoring previous 
timer settings. Additionally, "preempt_disable" and "preempt_enable" allow for 
temporarily blocking and unblocking the SIGVTALRM signal, enabling critical 
sections of code to run without interruption. This model introduces 
time-slicing into the threading system, facilitating simulated parallelism and 
enhancing overall responsiveness.

### Global 'old' timer variable

"struct itimerval old;": This variable stores the previous timer value before 
the new timer settings are applied. It is used to restore the old timer 
settings when preemption is stopped with preempt_stop(). struct itimerval 
contains two structures of type struct timeval: it_value for the current timer 
value and it_interval for the timer interval. Setting old allows the program 
to revert to the previous state of the timer, ensuring that changes made for 
preemption do not permanently alter the process's behavior.

## Implementation

Users can leverage this user-level threading code to create, manage, and execute 
lightweight threads within their applications, allowing for concurrent execution 
flows. The threading model is built around the concept of thread control blocks 
(TCBs), which encapsulates the state, context, and stack for each thread.

Users who use this implementation of threading to run their tasks and
processes have the option of enabling preemption. If the user wants to enable 
preemption in their threads:

1. They can set the first argument in "uthread_run" to TRUE, to enable
preemption by default in the threads.
2. They can enable or disable preemption in the middle of their code by
implementing the "preempt_enable(bool)" or the "preempt_disable()" line in their
code.

This allows the user to be able to split resources among all the threads and to
stop it if they want a certain task to be finished first.

## Memory Management

The memory deallocation approach in the provided threading code ensures 
efficient resource management by systematically reclaiming memory allocated to 
threads upon their completion and during system shutdown. When a thread 
finishes execution, "uthread_exit" deallocates its stack and control block 
(TCB) to free up the significant memory resources each thread consumes. 
This process includes deallocating the thread's execution stack and then 
freeing the TCB, which holds crucial thread information. Similarly, at the end 
of the program's execution, "uthread_run" performs cleanup for the idle thread 
by deallocating its stack and TCB, and also destroying the ready queue to ensure 
all dynamic memory associated with thread management is released. Additionally, 
the thread creation process incorporates robust error handling to preemptively 
free allocated resources in case of initialization failures, preventing memory 
leaks. This meticulous approach to memory deallocation is essential for 
maintaining the system's memory efficiency and preventing leaks in a 
multi-threaded environment, ensuring that resources are only used when 
necessary and promptly released when they are no longer needed.

## Tester code

### queue_tester.c

The queue_tester.c contains various types of tests for queue operations. It aims
to cover edge case operations which can lead to failure and program crashing.
Such test cases include test_contents, whose job is to run the enqueue operation
on a mass scale and proceed to validate each node's data field with the correct
data. This operation helps to ensure that the queue is taking in user-provided
data and enqueuing it properly. After this, we decided to test if the queue was
able to properly dequeue all its contents. This is tested by the function
test_dequeue. This function at first enqueues a bunch of data and after proceeds
to dequeue it. This is crucial in the sense of ensuring user data is properly 
deleted on command. This function utilizes the queue_iterate function to 
iteratively clear all data from the queue and after all is done utilizes the 
length function to return the emptied queue length. Another important aspect of 
queue operations was the destroy. This is tested by the function 
test_full_queue_destroy. What this function does is once enqueue some data and 
then using queue_destory, destroy the queue. However, it should return -1, the 
user is not allowed to destroy a full queue. There are many other test cases 
like the ones shown above with variations, to get a full comprehensive coverage 
of queue operations.

### test_preempt.c

The "test_preempt.c" contains code to test the function of preemption in
threads. Four threads in the code will each calculate fibonacci 
sequences starting at 0 and ending at 40. Since calculating fibonacci numbers 
at higher numbers using this method is very intensive, it is a good method to 
test preemption in threads. The main function starts the first thread with 
preemption disabled. Thread 1 will keep running until the 35th value in the 
fibonacci sequence is starting to be calculated. Preemption will be enabled 
which allows threads 2, 3, and 4 to start working instead of thread 1 hogging 
all the resources. To test the disabling of preemption, when thread 4 is 
calculating the 35th fibonacci number, preemption is disabled, and thread 4 will 
continue to use all the resources until it is finished. Then according to the 
queue, the threads will finish calculating one by one until thread 1 is 
finished calculating its 40th fibonacci number.
