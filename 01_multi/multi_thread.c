#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

pthread_t tid[2];
int counter;

void* trythis(void* arg)
{
    unsigned long i = 0;
    counter += 1;
    printf("\n Job %d has started\n", counter);

    for (i = 0; i < (0xFFFFFFFF); i++)
        ;
    printf("\n Job %d has finished\n", counter);

    return NULL;
}

int main(void)
{
    int i = 0;
    int error;

    while (i < 2) {
        error = pthread_create(&(tid[i]), NULL, &trythis, NULL);
        if (error != 0)
            printf("\nThread can't be created : [%s]", strerror(error));
        i++;
    }

    pthread_join(tid[0], NULL);
    pthread_join(tid[1], NULL);

    return 0;
}

/* Why it has occurred ?
On observing closely and visualizing the execution of the code, we can see that :

The log ‘Job 2 has started’ is printed just after ‘Job 1 has Started’ so it can easily be concluded that while thread 1 was processing the scheduler scheduled the thread 2.

If we take the above assumption true then the value of the ‘counter’ variable got incremented again before job 1 got finished.

So, when Job 1 actually got finished, then the wrong value of counter produced the log ‘Job 2 has finished’ followed by the ‘Job 2 has finished’ for the actual job 2 or vice versa as it is dependent on scheduler.

So we see that its not the repetitive log but the wrong value of the ‘counter’ variable that is the problem.

The actual problem was the usage of the variable ‘counter’ by a second thread when the first thread was using or about to use it.

In other words, we can say that lack of synchronization between the threads while using the shared resource ‘counter’ caused the problems or in one word we can say that this problem happened due to ‘Synchronization problem’ between two threads.


How to solve it ?

The most popular way of achieving thread synchronization is by using Mutexes.
*/


