/*
 * Counters
 * */
int NumBays;
int numDA;
int numDB;
int numDO;






typedef enum {DA, DB, DO} dogtype;

/*
 * This function is called to initialize any variables your
 * solution requires. It takes as its argument the number of wash bays in the dog wash facility. The
 * function should return 0 on success and -1 for a failure return.
 */

int dogwash_init(int numbays);


/*
 * This function is called when a new dog (of type dogtype)
 * arrives at the dog wash needing a wash. The calling thread should block until the new dog can be
 * allocated a wash bay. The function should return 0 on success and -1 for a failure return.
 */

int newdog(dogtype dog);


/*
 * This function is called when a dog of type dogtype that had
 * been allocated a wash bay is done being washed and the wash bay is now free. The function should
 * return 0 on success and -1 for a failure return.
 */

int dogdone(dogtype dog);




/*
 * This function is called to perform any required data cleanup actions;
 * after calling this function it should be possible to reinitialize (using dogwash_init()) and make
 * another sequence of calls to newdog() and dogdone(). The function should return 0 on success
 * and -1 for a failure return.
 */
int dogwash_done();






