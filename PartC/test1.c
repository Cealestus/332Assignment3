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
	//Create dogs to use for testing
	dogtype dogA = DA;
	dogtype dogA2 = DA;
	dogtype dogB = DB;
	dogtype dogB2 = DB;
	dogtype dogB3 = DB;
	dogtype dogB4 = DB;
	dogtype dogB5 = DB;
	dogtype dogB6 = DB;
	dogtype dogB7 = DB;
	dogtype dogO = DO;
	dogtype dogO2 = DO;
	dogtype dogO3 = DO;
	dogtype dogO4 = DO;

	//Create the threads to handle the dogs
	pthread_t create_dogA;
	pthread_t create_dogA2;
	pthread_t create_dogB;
	pthread_t create_dogB2;
	pthread_t create_dogB3;
	pthread_t create_dogB4;
	pthread_t create_dogB5;
	pthread_t create_dogB6;
	pthread_t create_dogB7;
	pthread_t create_dogO;
	pthread_t create_dogO2;
	pthread_t create_dogO3;
	pthread_t create_dogO4;

	//Initialize the system to allow 5 dogs at a time
	dogwash_init(5);

	//Create all the threads to create dogs
	//Note that the order is specified to show the affects of starvation
	pthread_create(&create_dogB, NULL, create, (void*)dogB);
	pthread_create(&create_dogB2, NULL, create, (void*)dogB2);
	pthread_create(&create_dogB3, NULL, create, (void*)dogB3);
	pthread_create(&create_dogA, NULL, create, (void*)dogA);
	pthread_create(&create_dogO, NULL, create, (void*)dogO);
	pthread_create(&create_dogB4, NULL, create, (void*)dogB4);
	pthread_create(&create_dogB5, NULL, create, (void*)dogB5);
	pthread_create(&create_dogO2, NULL, create, (void*)dogO2);
	pthread_create(&create_dogO3, NULL, create, (void*)dogO3);
	pthread_create(&create_dogB6, NULL, create, (void*)dogB6);
	pthread_create(&create_dogA2, NULL, create, (void*)dogA2);
	pthread_create(&create_dogO4, NULL, create, (void*)dogO4);
	pthread_create(&create_dogB7, NULL, create, (void*)dogB7);

	//Wait until all the threads have completed to run the rest of main()
	pthread_join(create_dogB, NULL);
	pthread_join(create_dogB2, NULL);
	pthread_join(create_dogB3, NULL);
	pthread_join(create_dogA, NULL);
	pthread_join(create_dogO, NULL);
	pthread_join(create_dogB4, NULL);
	pthread_join(create_dogB5, NULL);
	pthread_join(create_dogO2, NULL);
	pthread_join(create_dogO3, NULL);
	pthread_join(create_dogB6, NULL);
	pthread_join(create_dogA2, NULL);
	pthread_join(create_dogO4, NULL);
	pthread_join(create_dogB7, NULL);

	//Let the user know that all the threads have joined and main has continued to run
	printf("In main after completing\n");

	return 0;
}
