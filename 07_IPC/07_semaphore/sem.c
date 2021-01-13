// C program to demonstrate working of Semaphores 
#include <stdio.h> 
#include <pthread.h> 
#include <semaphore.h> 
#include <unistd.h> 

sem_t mutex; 

void* thread(void* arg) 
{ 
	//wait 
	sem_wait(&mutex); 
	printf("\nEntered..\n"); 

	//critical section 
	sleep(4); 
	
	//signal 
	printf("\nJust Exiting...\n"); 
	sem_post(&mutex); 
} 


int main() 
{ 
	/* pshared = 0: is shared between threads.
	   pshared # 0: is shared between processes.
	   value: number of key. ie: 2 -> có 2 thread đc access đồng thời */  
	sem_init(&mutex, 0, 0);
	pthread_t t1,t2,t3; 
	pthread_create(&t1,NULL,thread,NULL); 
	sleep(1); 
	pthread_create(&t2,NULL,thread,NULL); 
	sleep(1); 
	pthread_create(&t2,NULL,thread,NULL); 
	pthread_join(t1,NULL); 
	pthread_join(t2,NULL); 
	pthread_join(t3,NULL); 
	sem_destroy(&mutex); 
	return 0; 
} 



/* Output and explaination

$ gcc -o sem sem.c -lpthread -lrt
$ ./sem

-->
Entered..		// t1 vao`

Entered..		// t2 vao`

Just Exiting...		// t1 ra

Entered..		// t3 vào ( vì semaphore chỉ có init 2 key -> chờ nhả 1 key là vào liền)

Just Exiting...		// t2 ra

Just Exiting...		// t3 ra
*/

