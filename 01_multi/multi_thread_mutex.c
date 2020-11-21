#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <unistd.h>

pthread_t tid[2];
int counter;
pthread_mutex_t lock = PTHREAD_MUTEX_INITIALIZER;

void* trythis(void* arg)
{
	pthread_mutex_lock(&lock);
//printf("\n lock._data._count: %d\n", lock.__data.__count);
	unsigned long i = 0;
	counter += 1;
	printf("\n Job %d has started\n", counter);

	for (i = 0; i < (0xFFFFFFFF); i++)
		;

	printf("\n Job %d has finished\n", counter);

	pthread_mutex_unlock(&lock);

	return NULL;
}

int main(void)
{
	int i = 0;
	int error;

	// if (pthread_mutex_init(&lock, NULL) != 0) {
	// 	printf("\n mutex init has failed\n");
	// 	return 1;
	// }

	while (i < 2) {
		error = pthread_create(&(tid[i]),
							NULL,
							&trythis, NULL);
		if (error != 0)
			printf("\nThread can't be created :[%s]",
				strerror(error));
		i++;
	}
 //sleep(20); 
	 //pthread_join(tid[0], NULL); // thread1 start after creating
	 pthread_join(tid[1], NULL); // time that thread2 blocking main thread that is enough for thread1 end.
	 // this action is NOT available if only use pthread_join(tid[0]).
	 // Because time block of thread1 that is NOT enough for thread2 end.
puts("\nhahahahhahahaha\n");
	//pthread_mutex_destroy(&lock);

	return 0;
}

