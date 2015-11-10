#include <pthread.h>
#include "dogwashsynch.h"
#include <semaphore.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*Semaphores*/
sem_t SA;
sem_t SB;
sem_t SBAYS;
sem_t SNUMBAYS;


/*
 * Function to handle dogs of type A
 * */
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
	return 0;
}

/*
 * Function to handle dogs of type B
 */
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
	return 0;
}

/*
 * Function to handle dogs of type O
 */
void *DogOThread(void *arg){
	printf("In DogO\n");
	sem_wait(&SNUMBAYS);
	while(NumBays == 0){
		sem_post(&SNUMBAYS);
		sem_wait(&SNUMBAYS);
	}
	NumBays--;
	sem_post(&SNUMBAYS);
	return 0;
}

/*
 * This function is called to initialize any variables your
 * solution requires. It takes as its argument the number of wash bays in the dog wash facility. The
 * function should return 0 on success and -1 for a failure return.
 */
int
dogwash_init(int numbays) {

	NumBays = numbays;
	numDA = 0;
	numDB = 0;
	numDO = 0;

	sem_init(&SA, 0, 1);
	sem_init(&SB, 0, 1);
	sem_init(&SBAYS, 0, 1);
	sem_init(&SNUMBAYS, 0, 1);

	return 0;
}

/*
 * This function is called when a new dog (of type dogtype)
 * arrives at the dog wash needing a wash. The calling thread should block until the new dog can be
 * allocated a wash bay. The function should return 0 on success and -1 for a failure return.
 */
int 
newdog(dogtype dog) {
	if(dog == DA){
		pthread_t DogA_Thread;
		printf("Creating DogA\n");
		pthread_create(&DogA_Thread, NULL, DogAThread, NULL);
	}
	else if(dog == DB){
		pthread_t DogB_Thread;
		printf("Creating DogB\n");
		pthread_create(&DogB_Thread, NULL, DogBThread, NULL);
	}
	else if(dog == DO){
		pthread_t DogO_Thread;
		printf("Creating DogO\n");
		pthread_create(&DogO_Thread, NULL, DogOThread, NULL);
	}
	return 0;	
}



/*
 * This function is called when a dog of type dogtype that had
 * been allocated a wash bay is done being washed and the wash bay is now free. The function should
 * return 0 on success and -1 for a failure return.
 */
int 
dogdone(dogtype dog) {
	if(dog == DA){
		printf("Dogdone A\n");
		sem_wait(&SNUMBAYS);
		NumBays++;
		sem_post(&SNUMBAYS);
		sem_wait(&SA);
		numDA--;
		if(numDA == 0){
			sem_post(&SBAYS);
		}
		sem_post(&SA);
	}
	else if(dog == DB){
		printf("Dogdone B\n");
		sem_wait(&SNUMBAYS);
		NumBays++;
		sem_post(&SNUMBAYS);
		sem_wait(&SB);
		numDB--;
		if(numDB == 0){
			sem_post(&SBAYS);
		}
		sem_post(&SB);
	}
	else if(dog == DO){
		printf("Dogdone O\n");
		sem_wait(&SNUMBAYS);
		NumBays++;
		sem_post(&SNUMBAYS);
	}
	else{
		printf("ERROR, Not one of the 3 dog types\n");
	}
	return 0;
}


/*This function is called to perform any required data cleanup actions;
 * after calling this function it should be possible to reinitialize (using dogwash_init()) and make
 * another sequence of calls to newdog() and dogdone(). The function should return 0 on success
 * and -1 for a failure return
 */
int 
dogwash_done() {
	return 0;
}
