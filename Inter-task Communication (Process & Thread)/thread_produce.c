#include <stdbool.h>
#include <string.h>
#include <stdio.h>
#include <stdlib.h>
#include <time.h>
#include <sys/time.h>
#include <mqueue.h>
#include <sys/stat.h>
#include <pthread.h>
#include <semaphore.h>
#include <signal.h>
#include <math.h>
//#include <.h>
//#include <.h>

// global var
sem_t spaces;
sem_t items;
int BUFFER_SIZE; 
int INT_NEED_PRODUCED;
int NUM_OF_CONSUMERS;
int NUM_OF_PRODUCERS;
int *buffer;
int produced_counter;
int consumed_counter;
int buffer_index;
pthread_mutex_t mutex;
struct timeval tv;
double t1;
double t2;



//produce
void *producer( void* arg){
	int pid = *(int*)arg;
	// free ( arg );	
	// arg = NULL;
	//produced_counter = *pid;
	//int pindex = 0;
	while(INT_NEED_PRODUCED > produced_counter && pid < INT_NEED_PRODUCED) {
		// if(INT_NEED_PRODUCED <= produced_counter){
		// 	//Produce finished
		// 	pthread_exit(NULL);
		// }
		//if(){//Produce cond need to be filled
			//starts to produce
		//if(INT_NEED_PRODUCED % NUM_OF_PRODUCERS == id){
			// if (*pid > INT_NEED_PRODUCED){
			// 	break;
			// }
			int product;
			// produced_counter = *pid;
			product = produced_counter;

			if(product % NUM_OF_PRODUCERS == pid){
				sem_wait( &spaces );
				pthread_mutex_lock(&mutex);
				// produced_counter += NUM_OF_PRODUCERS;
				// buffer[pindex] = v;
				// pindex = (pindex + 1) % BUFFER_SIZE;
				// buffer change/ cirtical
				buffer[buffer_index] = product;
				//printf("producer %d produce %d buffer # %d # of P %d\n", *pid, product, buffer_index, NUM_OF_PRODUCERS);
				buffer_index ++;
				produced_counter ++;
				pthread_mutex_unlock(&mutex);
				sem_post( &items );
			}
		//}
		//}
	}
	//produced_counter = 0;
	pthread_exit( NULL );
}

//comsumer
void* consumer( void* arg ) {
	int cid = *(int*)arg;
	// free ( arg );
	// arg = NULL;
	int product_int;
	int result;
	// int cindex = 0;
	// int ctotal = 0;
	//printf("%d", *cid);
	while(buffer_index >= 0) {
		sem_wait( &items );
		pthread_mutex_lock(&mutex);
		buffer_index --;
		if(buffer_index < 0){
			pthread_mutex_unlock(&mutex);
			sem_post( &items );
			pthread_exit( NULL );
		}
		product_int = buffer[buffer_index];
		//printf("consumer %d consumed %d buffer # %d\n", *cid, product_int, buffer_index);
		consumed_counter ++;
		if(consumed_counter > INT_NEED_PRODUCED - 1){
			sem_post( &items );
		}
		//buffer_index --;
		pthread_mutex_unlock(&mutex);
		sem_post( &spaces );

		result = sqrt(product_int);
		if(product_int == (result * result)){
			printf("%d %d %d\n", cid, product_int, result);
		}
		// consume( temp );
		// ++ctotal;
	}
	// if (buffer_index == -1){
	// 	buffer_index = 0;
	// }
  	pthread_exit( NULL );
}
//main		
int main(int argc,  char const *argv[])
{
	//printf("1\n");
	buffer_index = 0;
	produced_counter = 0;
	consumed_counter = 0;

	//N the number of integers the producers should produce in total 1
	//B be the buffer size 2
	//P be the number of producers 3
	//C be the number of consumers 4
	if (argc != 5){
		printf("Input Error!/n");
		return -1;
	}

	BUFFER_SIZE = atoi(argv[2]);
	if(BUFFER_SIZE < 0){
		printf("Buffer Size Error!/n");
		return -1;
	}
	NUM_OF_PRODUCERS = atoi(argv[3]);
	NUM_OF_CONSUMERS = atoi(argv[4]);

	if(NUM_OF_PRODUCERS < 1 ){
		printf("Number of Producer Error!/n");
		return -1;
	}

	if(NUM_OF_CONSUMERS < 1 ){
		printf("Number of Consumers Error!/n");
		return -1;
	}

	if(NUM_OF_CONSUMERS > 1 && NUM_OF_PRODUCERS > 1){
		printf("Bad Case, Producer and Consumer Both Bigger Than 1!/n");
		return -1;
	}

	INT_NEED_PRODUCED = atoi(argv[1]);
	if(INT_NEED_PRODUCED < 0){
		printf("Number of Integer Need to Produce Error!/n");
		return -1;
	}

	//printf("2\n");
	//int **arr = (int **)malloc(r* sizeof(int *));
	/* code */
	//produced_counter = 0;
	//buffer = malloc( BUFFER_SIZE * sizeof( int ) );
	buffer = (int *) malloc(sizeof(int) *BUFFER_SIZE);
	int *pid_array = (int *) malloc(sizeof(int) *NUM_OF_PRODUCERS);
	int *cid_array = (int *) malloc(sizeof(int) *NUM_OF_CONSUMERS);
	int i;
	for ( i = 0; i < BUFFER_SIZE; i++ ) {
		buffer[i] = -1;
	}
	for ( i = 0; i < NUM_OF_PRODUCERS; i++ ) {
		pid_array[i] = i;
	}
	for ( i = 0; i < NUM_OF_CONSUMERS; i++ ) {
		cid_array[i] = i;
	}

	//printf("3\n");
	//initiallize mutex
	pthread_mutex_init(&mutex, NULL);
	pthread_mutex_init(&mutex, NULL);
    //initiallize sem
	sem_init( &spaces, 0, BUFFER_SIZE );
	sem_init( &items, 0, 0 );
	//array for threads
	pthread_t producer_array[NUM_OF_PRODUCERS];
	pthread_t consumer_array[NUM_OF_CONSUMERS];

	//printf("4\n");
	//starting point
	gettimeofday(&tv, NULL);
	t1 = tv.tv_sec + tv.tv_usec/1000000.0;

	//printf("5\n");
	//PC
	//create threads
	int a;
	for(a = 0; a < NUM_OF_PRODUCERS; a++){
		pthread_create(&producer_array[a], NULL, producer, &pid_array[a]);
	}
	//printf("6\n");
	for(a = 0; a < NUM_OF_CONSUMERS; a++){
		pthread_create(&consumer_array[a], NULL, consumer, &cid_array[a]);
	}
	//printf("7\n");

	//Join
	for(a = 0; a < NUM_OF_PRODUCERS; a++){
		pthread_join(producer_array[a], NULL);
	}
	//printf("8\n");
	for(a = 0; a < NUM_OF_CONSUMERS; a++){
		pthread_join(consumer_array[a], NULL);
	}

	//printf("9\n");
	//ending point
	gettimeofday(&tv, NULL);
	t2 = tv.tv_sec + tv.tv_usec/1000000.0;
	printf("System execution time: %.6lf\n", t2-t1);

	buffer = NULL;
	free( buffer );
	buffer = NULL;
	free( pid_array );
	free( cid_array );
	sem_destroy( &spaces );
	sem_destroy( &items );
	pthread_mutex_destroy( &mutex );

	//printf("10\n");
	//pthread_exit(0);
	return 0;
}

