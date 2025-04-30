/*************************************************************
----- 48450 -- week 6 lab ------ 
By programing the file, you will follow the lecture notes to implement the CPU scheduling policy changing. 
*************************************************************/

#include <errno.h>		/* errno and error codes */
#include <sys/time.h>		/* for gettimeofday() */
#include <unistd.h>		/* for gettimeofday(), getpid() */
#include <stdio.h>		/* for printf() */
#include <unistd.h>		/* for fork() */
#include <sys/types.h>		/* for wait() */
#include <sys/wait.h>		/* for wait() */
#include <signal.h>		/* for kill(), sigsuspend(), others */
#include <sys/ipc.h>		/* for all IPC function calls */
#include <sys/shm.h>		/* for shmget(), shmat(), shmctl() */
#include <sys/sem.h>		/* for semget(), semop(), semctl() */
#include <sched.h>		/* for scheduling calls/constants */
#include <sys/resource.h>
#include <stdlib.h>

void printSchedulingPolicy(void);


int main(void)
{     

  struct sched_param p;

	printSchedulingPolicy();
	printf("the current priority = %d\n",p.sched_priority);
/* change the CPU scheduling policy*/
	
/* experiment with value of priority below: 1) run =0, 2) try a high value such as 99 */
	p.sched_priority = 99;   

/************************************** 
You can replace this code line with 'if (sched_setscheduler(0, SCHED_RR, &p) != 0)' {
****************************************/

	if (sched_setscheduler(0, SCHED_FIFO, &p) == -1) {
		printf("Failed to set the scheduler, please use sudo\n");exit(1);	}	   	
	printf("After change policy: ");
	printSchedulingPolicy();

	exit(0);
}/* end of Main */
	

/********************Function Call *******************/

/*print out the current CPU scheduling policy in operation system*/

void printSchedulingPolicy(void)
{
  	int which;
	/* get the current CPU scheduling policy */
	
	which = sched_getscheduler(0);
	switch (which) {
	case SCHED_OTHER: 
			printf("default scheduling is being used\n");		
		  	break;
	case SCHED_FIFO:
			printf("fifo scheduling is being used\n");		
		  	break;
	case SCHED_RR:		
			printf("round robin scheduling is being used\n");		
		  	break;
	}		
}
