#include "dogwashsynch.h"
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>


int main(int argc, char* argv[]){
	dogtype dogA = DA;
	dogtype dogB = DB;
	dogtype dogO = DO;

	dogwash_init(2);
	newdog(dogA);
	newdog(dogB);
	newdog(dogO);
	sleep(4);
	return 0;
}
