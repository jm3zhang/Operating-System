#include <stdio.h>
#include <stdlib.h>
#include <math.h>
#include <sys/time.h>
#include <unistd.h>
#include <mqueue.h>

struct timeval tv;
double t1;
double t2;

mqd_t producer;
mqd_t consumer;
mqd_t publicQueue;

int Number, Bsize, Pnum, Cnum;
int empty = -1;

int Produce(int id)
{
  int currentMsgNum, spaceAvalibal, msg, index;
  msg = 1;
  index = 0;
  while(1)
  {
	//printf("P %d - wait \n", id);
    mq_receive(producer, (char *)&spaceAvalibal, sizeof(int), 0);
	//printf("P %d - get \n", id);
    mq_receive(publicQueue, (char *)&currentMsgNum, sizeof(int), 0);	//current count
    if (currentMsgNum > 0) {
		msg = id + index*Pnum;
		//printf("%d \n", msg);
		index++;                // create some msg
		currentMsgNum--;        // global counter --
		mq_send(publicQueue, (char *)&currentMsgNum, sizeof(int), 0);
		mq_send(consumer, (char *)&msg, sizeof(int), 0);
    }
	else if (currentMsgNum <= 0){
		mq_send(producer, (char *)&spaceAvalibal, sizeof(int), 0);  // Let the other producers get chance to stop
		mq_send(consumer, (char *)&empty, sizeof(int), 0);
		mq_send(publicQueue, (char *)&currentMsgNum, sizeof(int), 0);
		break;
	}
	
  }
	//printf(" finished producer %d \n", id);
  return 0;
}

int Consume(int id)
{
  int msg, spaceAvalibal, output;
  spaceAvalibal = 1;
  while (1) {
	  //printf("C %d - wait \n", id);
    mq_receive(consumer, (char *)&msg, sizeof(int), 0);
	//printf("C %d - get \n", id);
    if (msg != empty) {
		
		output = sqrt(msg);
		if(output * output == msg){
			printf("%d %d %d\n", id, msg, output);
		}
		mq_send(producer, (char *)&spaceAvalibal, sizeof(int), 0);	
		
    } else if(msg == empty){
		mq_send(consumer, (char *)&msg, sizeof(int), 0);
		break;
	}
    
  }
	//printf(" finished consumer %d \n", id);
  return 0;
}

int main(int argc, char *argv[])
{
	//printf("start\n");
    //  A valid posix queue name must start with a "/".

    // int Number, Bsize, Pnum, Cnum;
    int P_pid, C_pid, Pid;                 // producer Pid and consumer Pid, and a public used Pid
    int currentMsgNum;                // receive the number of how many msg left to be produce in Global Counter
    int i;
	
    char *pq_name = "/promail1";
	char *cq_name = "/conmail1";
	char *gq_name = "/pmail1";
	
	// unlink all the queues to make sure they are cleaned.
    mq_unlink("/promail1");
    mq_unlink("/conmail1");
    mq_unlink("/pmail1");
    

    if (argc != 5 || atoi(argv[1]) < 0 || atoi(argv[2]) < 0 || atoi(argv[3]) < 0 || atoi(argv[4]) < 0) {
        perror("Invalid arguments \n");
        return 0;
    }

    Number = atoi(argv[1]);  //number of messages the producers should produce
    Bsize = atoi(argv[2]);   //buffer size
    Pnum = atoi(argv[3]);    //number of producers
    Cnum = atoi(argv[4]);   // number of consumers

    
    struct mq_attr attr; // queue attr for Producer and Receiver
    attr.mq_maxmsg  = Bsize;
    attr.mq_msgsize = sizeof(int);
    attr.mq_flags   = 0;	/* a blocking queue  */

	mode_t mode = S_IRUSR | S_IWUSR;

   // printf("initial the three queues \n");
    producer = mq_open(pq_name, O_RDWR | O_CREAT, mode, &attr);
    if (producer == -1 ) {
		perror("mq_open1() failed");
		return 0;
	}
    consumer = mq_open(cq_name, O_RDWR | O_CREAT, mode, &attr);
    if (consumer == -1 ) {
		perror("mq_open2() failed");
		return 0;
	}
	
	
	struct mq_attr attr2; // queue attr for Global Queue
    attr2.mq_maxmsg  = 1;
    attr2.mq_msgsize = sizeof(int);
    attr2.mq_flags   = 0;	/* a blocking queue  */

	
    publicQueue = mq_open(gq_name, O_RDWR | O_CREAT, mode, &attr2);
    if (publicQueue == -1 ) {
		perror("mq_open3() failed");
		return 0;
	}

    //start time
 	//printf("Timer start \n");   
    gettimeofday(&tv, NULL);
	  t1 = tv.tv_sec + tv.tv_usec/1000000.0;
	  
    //printf("initial the buffer sized box \n");
    // set Buffer sized msg box
    for (i = 0; i < Bsize; i++) {
    	if(( mq_send(producer, (char *)&i, sizeof(int), 0)) == -1){
    		perror("mq_send() for producer failed");
    	}
     
    }
    
    //printf("initial the global counter \n");
    // set total number of msgs - global counter
    if(( mq_send(publicQueue, (char *)&Number, sizeof(int), 0)) == -1 ){
    	perror("mq_send() for publicQueue failed");
    }
   


	
	//printf("start fork produce processes\n");
    for(P_pid = 0; P_pid < Pnum; P_pid++)
    {
      Pid = fork();
      if (Pid == 0) {
        Produce(P_pid);
		return 0;
      } else if (Pid < 0) {
        printf("Fail to fork produce process %d \n", P_pid );
		return 0;
      }
    }

	//printf("start fork consumer processes\n");
    for(C_pid = 0; C_pid < Cnum; C_pid++)
    {
      Pid = fork();
      if (Pid == 0) {
        Consume(C_pid);
		return 0;
      } else if (Pid < 0) {
        printf("Fail to fork consume process %d \n", P_pid );
		return 0;
      }
    }

    while (1) {
      //Polling: keep tracking the number of msg left in the global Counter
      mq_receive(publicQueue, (char *)&currentMsgNum, sizeof(int), 0); //get
      if (currentMsgNum > 0) {
        mq_send(publicQueue, (char *)&currentMsgNum, sizeof(int), 0); //set
        //continue...
      } else if (currentMsgNum <= 0) {
		  mq_send(publicQueue, (char *)&currentMsgNum, sizeof(int), 0);
        break;
        // finished
      }
    }
	
	
	if (mq_close(producer) == -1){
		perror("mq_close() producer failed");
		return 0;	
	}	
	if (mq_close(consumer) == -1){
		perror("mq_close() consumer failed");
		return 0;
	}
	if (mq_close(publicQueue) == -1){
		perror("mq_close() publicQueue failed");
		return 0;
	}

	if (mq_unlink(pq_name) != 0){
		perror("mq_unlink() producer failed");
		return 0;
	}
	
	if (mq_unlink(cq_name) != 0){
		perror("mq_unlink() consumer failed");
		return 0;
	}
	
	if (mq_unlink(gq_name) != 0){
		perror("mq_unlink() publicQueue failed");
		return 0;
	}


    //end time
    gettimeofday(&tv, NULL);
	  t2 = tv.tv_sec + tv.tv_usec/1000000.0;

    double runtime = t2 - t1;
    printf("System execution time: %.6lf seconds \n", runtime);
	// printf("%.6lf \n", runtime);
    return 0;
  }
