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
	//Wait until semaphore for dogs of type A is free
	sem_wait(&SA);
	//Increment number of A dogs currently in the system
	numDA++;
	if(numDA == 1){
		//If there used to be 0 dogs in the system, wait until the bays are free
		sem_wait(&SBAYS);
	}
	//Release semaphore for dogs of type A
	sem_post(&SA);
	//Wait until the number of bays semaphore is free
	sem_wait(&SNUMBAYS);
	while(NumBays == 0){i
		//While none of the bays are free, acquire and release the number of bays lock
		sem_post(&SNUMBAYS);
		sem_wait(&SNUMBAYS);
	}
	//Decrease the number of free bays
	NumBays--;
	//Useful for user testing & feedback
	printf("In DogA\n");
	//Release the semaphore for the number of bays
	sem_post(&SNUMBAYS);
	return 0;
}

/*
 * Function to handle dogs of type B
 * The comments for DogAThread are exactly the same for DogBThread, and are thus not included here
 */
void *DogBThread(void *arg){
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
	printf("In DogB\n");
	sem_post(&SNUMBAYS);
	return 0;
}

/*
 * Function to handle dogs of type O
 */
void *DogOThread(void *arg){
	//Wait until the semaphore for the number of bays is free
	sem_wait(&SNUMBAYS);
	while(NumBays == 0){
		//While there are no free bays, continuously release and acquire the semaphore for number of bays
		sem_post(&SNUMBAYS);
		sem_wait(&SNUMBAYS);
	}
	//Decrease the number of available free bays
	NumBays--;
	//Useful for user testing & feedback
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
	//Create the variables for the system
	int check;
	NumBays = numbays;
	numDA = 0;
	numDB = 0;
	numDO = 0;

	//The following checks are used to see if the semaphore initializations complete successfully
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
	//If the dogs are of type A
	if(dog == DA){
		pthread_t DogA_Thread;
		printf("Creating DogA\n");
		//Check to see if thread creation was successful. If not, return
		check = pthread_create(&DogA_Thread, NULL, DogAThread, NULL);
		if(check != 0){
			return -1;
		}
		//Wait until the called thread completes, before returning back to the calling function
		check = pthread_join(DogA_Thread, NULL);
		if(check != 0){
			return -1;
		}
		return 0;
	}
	//Otherwise, if the dog is of type B
	else if(dog == DB){
		pthread_t DogB_Thread;
		printf("Creating DogB\n");
		//Check to see if thread creation was successful, otherwise return with a fail
		check = pthread_create(&DogB_Thread, NULL, DogBThread, NULL);
		if(check != 0){
			return -1;
		}
		//Wait until the called thread completes, before returning back to the calling function
		check = pthread_join(DogB_Thread, NULL);
		if(check != 0){
			return -1;
		}
		return 0;
	}
	//Lastly, if the dog is of type O
	else if(dog == DO){
		pthread_t DogO_Thread;
		printf("Creating DogO\n");
		//Check to see if thread creation was successful, otherwise return with a fail
		check = pthread_create(&DogO_Thread, NULL, DogOThread, NULL);
		if(check != 0){
			return -1;
		}
		//Wait until the called thread completes, before returning back to the calling function
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
	//If the dog is of type A
	if(dog == DA){
		//Let the user know that a dog of type A has completed
		printf("Dogdone A\n");
		//Wait to acquire the semaphore for the number of bays
		sem_wait(&SNUMBAYS);
		//Increment the number of free bays
		NumBays++;
		//Release the semaphore for the number of bays
		sem_post(&SNUMBAYS);
		//Wait until the semaphore for dogs of type A is available
		sem_wait(&SA);
		//Decrease the number of dogs of type A in the system
		numDA--;
		if(numDA == 0){
			//If there are now no more dogs of type A in the system, release the bays semaphore
			sem_post(&SBAYS);
		}
		//Release the semaphore for dogs of type A
		sem_post(&SA);
	}
	//Dogs of type B has the same code as dog A, so can be referenced as above
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
	//Else if completed dog is of type O
	else if(dog == DO){
		//Let the user know that a dog of type O has completed
		printf("Dogdone O\n");
		//Wait until the semaphore for number of bays is available
		sem_wait(&SNUMBAYS);
		//Increment the number of free bays
		NumBays++;
		//Release the semaphore for number of free bays
		sem_post(&SNUMBAYS);
	}
	else{
		//If we hit here, something went wrong, return failure
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
	//Reinitialize all the number of dogs of type A, B, and O to zero to prep the system for running again
	numDA = 0;
	numDB = 0;
	numDO = 0;

	return 0;
}
