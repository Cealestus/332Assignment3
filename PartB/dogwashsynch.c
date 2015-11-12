#include <pthread.h>
#include "dogwashsynch.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

/*Semaphores*/
pthread_mutex_t SA;
pthread_mutex_t SB;
pthread_mutex_t SBAYS;
pthread_mutex_t SNUMBAYS;


/*
 * Function to handle dogs of type A
 * */
void *DogAThread(void *arg){
	pthread_mutex_ock(&SA);
	numDA++;
	if(numDA == 1){
		sem_wait(&SBAYS);
	}
	sem_post(&SA);
	pthread_mutex_lock(&SNUMBAYS);
	while(NumBays == 0){
		sem_post(&SNUMBAYS);
		pthread_mutex_lock(&SNUMBAYS);
	}
	NumBays--;
	printf("In DogA\n");
	sem_post(&SNUMBAYS);
	return 0;
}

/*
 * Function to handle dogs of type B
 */
void *DogBThread(void *arg){
	pthread_mutex_lock(&SB);
	numDB++;
	if(numDB == 1){
		pthread_mutex_lock(&SBAYS);
	}
	sem_post(&SB);
	pthread_mutex_lock(&SNUMBAYS);
	while(NumBays == 0){
		sem_post(&SNUMBAYS);
		pthread_mutex_lock(&SNUMBAYS);
	}
	NumBays--;
	printf("In DogB\n");
	sem_post(&SNUMBAYS);
	return 0;
}

/*
 * Function to handle dogs of type O
 */
void *DogOThread(void *arg){
	pthread_mutex_lock(&SNUMBAYS);
	while(NumBays == 0){
		sem_post(&SNUMBAYS);
		pthread_mutex_lock(&SNUMBAYS);
	}
	NumBays--;
	printf("In DogO\n");
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
	int check;
	NumBays = numbays;
	numDA = 0;
	numDB = 0;
	numDO = 0;

	check = sem_init(&SA, 0, 1);
	if(check != 0){
		return -1;
	}
	check = sem_init(&SB, 0, 1);
	if(check != 0){
		return -1;
	}
	check = sem_init(&SBAYS, 0, 1);
	if(check != 0){
		return -1;
	}
	check = sem_init(&SNUMBAYS, 0, 1);
	if(check != 0){
		return -1;
	}

	return 0;
}

/*
 * This function is called when a new dog (of type dogtype)
 * arrives at the dog wash needing a wash. The calling thread should block until the new dog can be
 * allocated a wash bay. The function should return 0 on success and -1 for a failure return.
 */
int 
newdog(dogtype dog) {
	int check;
	if(dog == DA){
		pthread_t DogA_Thread;
		printf("Creating DogA\n");
		check = pthread_create(&DogA_Thread, NULL, DogAThread, NULL);
		if(check != 0){
			return -1;
		}
		check = pthread_join(DogA_Thread, NULL);
		if(check != 0){
			return -1;
		}
		return 0;
	}
	else if(dog == DB){
		pthread_t DogB_Thread;
		printf("Creating DogB\n");
		check = pthread_create(&DogB_Thread, NULL, DogBThread, NULL);
		if(check != 0){
			return -1;
		}
		check = pthread_join(DogB_Thread, NULL);
		if(check != 0){
			return -1;
		}
		return 0;
	}
	else if(dog == DO){
		pthread_t DogO_Thread;
		printf("Creating DogO\n");
		check = pthread_create(&DogO_Thread, NULL, DogOThread, NULL);
		if(check != 0){
			return -1;
		}
		check = pthread_join(DogO_Thread, NULL);
		if(check != 0){
			return -1;
		}
		return 0;
	}
	return -1;	
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
		pthread_mutex_lock(&SNUMBAYS);
		NumBays++;
		sem_post(&SNUMBAYS);
		pthread_mutex_lock(&SA);
		numDA--;
		if(numDA == 0){
			sem_post(&SBAYS);
		}
		sem_post(&SA);
	}
	else if(dog == DB){
		printf("Dogdone B\n");
		pthread_mutex_lock(&SNUMBAYS);
		NumBays++;
		sem_post(&SNUMBAYS);
		pthread_mutex_lock(&SB);
		numDB--;
		if(numDB == 0){
			sem_post(&SBAYS);
		}
		sem_post(&SB);
	}
	else if(dog == DO){
		printf("Dogdone O\n");
		pthread_mutex_lock(&SNUMBAYS);
		NumBays++;
		sem_post(&SNUMBAYS);
	}
	else{
		return -1;
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
	numDA = 0;
	numDB = 0;
	numDO = 0;

	return 0;
}
