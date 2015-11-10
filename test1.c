#include "dogwashsynch.h"
#include <pthread.h>
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>

void *create(void *arg){
	dogtype dog = (dogtype) arg;
	if(dog == DA){
		printf("Create: DogA\n");
	}
	else if(dog == DB){
		printf("Create: DogB\n");
		
	}
	else if(dog == DO){
		printf("Create: DogO\n");
	}
	else{
		printf("It's not any of these\n");
	}
	newdog(dog);
	sleep(2);
	dogdone(dog);

	return 0;
}

int main(int argc, char* argv[]){
	dogtype dogA = DA;
	dogtype dogB = DB;
	dogtype dogO = DO;

	pthread_t create_dog;
	pthread_t create_dogB;
	pthread_t create_dogO;

	dogwash_init(5);
	pthread_create(&create_dog, NULL, create, (void*)dogA);
	pthread_create(&create_dogB, NULL, create, (void*)dogB);
	pthread_create(&create_dogO, NULL, create, (void*)dogO);

	pthread_join(create_dog, NULL);
	pthread_join(create_dogB, NULL);
	pthread_join(create_dogO, NULL);

	printf("In main after completing\n");

	return 0;
}
