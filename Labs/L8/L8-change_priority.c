#include <sys/time.h>
#include <sys/resource.h>
#include <sched.h>
#include <unistd.h>
#include <stdlib.h>
#include <stdio.h>
#include <pthread.h>


void printSchedulingPolicy();

int main(int argc, char **argv)
{     
  struct sched_param p;

	struct rusage usage;
	long long i;
	int setscheduler;
	
	/* Verify the correct number of arguments were passed in */
	if (argc != 2) {
		fprintf(stderr, "USAGE:sudo ./L8-Change X=[0,1,2] for scheduler switch\n");
	}
	 setscheduler = atoi(argv[1]); // convert a string to an integer
	
	printSchedulingPolicy();
	
	/* Try to set the p.sched_priority 0 and 99, what is the differnece in the result */
	p.sched_priority = 99;  
	 
	/* Switch the scheduling strategy to FIFO) or RR policies*/
	// add your program here
/************************************** 
You can replace this code line with 'if (sched_setscheduler(0, SCHED_RR, &p) != 0)' {
****************************************/

	switch (setscheduler) {
	case 0:
		printf("set the default scheduling automatically\n\n");	
		break;
	case 1:
		if (sched_setscheduler(0, SCHED_FIFO, &p) != 0) {
		printf("Failed to set the SCHED_FIFO scheduler\n");}	
		break;
	case 2:		
		if (sched_setscheduler(0, SCHED_RR, &p) != 0) {
		printf("Failed to set the SCHED_RR scheduler\n");}		
		break;
	}
  	
printf("After policy change: ");
	printSchedulingPolicy();

	exit(0);
}

void printSchedulingPolicy()
{
  	int which;
	which = sched_getscheduler(0);
	switch (which) {
	case SCHED_OTHER: 
			printf("default scheduling is being used\n\n");		
		  	break;
	case SCHED_FIFO:
			printf("fifo scheduling is being used\n\n");		
		  	break;
	case SCHED_RR:		
			printf("round robin scheduling is being used\n\n");		
		  	break;
	}		
}

