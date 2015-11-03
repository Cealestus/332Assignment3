#include <pthread.h>
#include "dogwashsynch.h"
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>

sem_t sem;
sem_t sem2;
sem_t sem3;
sem_t SA;
sem_t SB;
sem_t SBAYS;
sem_t SNUMBAYS;

void *DogAThread(void *arg){
	printf("In DogA\n");
	sem_wait(&SA);
	numDA++;
	if(numDA == 1){
		sem_wait(&SBAYS);
	}
	sem_post(&SA);
	sem_wait(&SNUMBAYS);
	while(NumBays == 0){
		sem_post(&SNUMBAYS);
		sem_wait(&SNUMBAYS);
	}
	NumBays--;
	sem_post(&SNUMBAYS);
	/*wait();*/
	sem_wait(&SNUMBAYS);
	NumBays++;
	sem_post(&SNUMBAYS);
	sem_wait(&SA);
	numDA--;
	if(numDA == 0){
		sem_post(&SBAYS);
	}
	sem_post(&SA);
	sem_post(&sem);
	return 0;
}

void *DogBThread(void *arg){
	printf("In DogB\n");
	sem_wait(&SB);
	numDB++;
	if(numDB == 1){
		sem_wait(&SBAYS);
	}
	sem_post(&SB);
	sem_wait(&SNUMBAYS);
	while(NumBays == 0){
		sem_post(&SNUMBAYS);
		sem_wait(&SNUMBAYS);
	}
	NumBays--;
	sem_post(&SNUMBAYS);
	/*wait();*/
	sem_wait(&SNUMBAYS);
	NumBays++;
	sem_post(&SNUMBAYS);
	sem_wait(&SB);
	numDB--;
	if(numDB == 0){
		sem_post(&SBAYS);
	}
	sem_post(&SB);
	sem_post(&sem2);
	return 0;
}

void *DogOThread(void *arg){
	printf("In DogO\n");
	sem_wait(&SNUMBAYS);
	while(NumBays == 0){
		sem_post(&SNUMBAYS);
		sem_wait(&SNUMBAYS);
	}
	NumBays--;
	sem_post(&SNUMBAYS);
	/*wait();*/
	sem_wait(&SNUMBAYS);
	NumBays++;
	sem_post(&SNUMBAYS);
	sem_post(&sem3);
	return 0;
}

int main(int argc, char* argv[]){
	pthread_t DogA_thread;
	pthread_t DogB_thread;
	pthread_t DogO_thread;
	dogwash_init(5);
	sem_init(&sem, 0, 0);
	sem_init(&sem2, 0, 0);
	sem_init(&sem3, 0, 0);
	sem_init(&SA, 0, 1);
	sem_init(&SB, 0, 1);
	sem_init(&SBAYS, 0, 1);
	sem_init(&SNUMBAYS, 0, 1);

	pthread_create(&DogA_thread, NULL, DogAThread, NULL);
	pthread_create(&DogB_thread, NULL, DogBThread, NULL);
	pthread_create(&DogO_thread, NULL, DogOThread, NULL);
	sem_wait(&sem);
	sem_wait(&sem2);
	printf("In main after creation of thread\n");
	return 0;
}
