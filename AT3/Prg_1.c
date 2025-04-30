/*******************************************************************************
The assignment 3 for subject 48450 (RTOS) in University of Technology Sydney(UTS)
This is a template of Program_1.c template. Please complete the code based on
the assignment 3 requirement. Assignment 3

------------------------------Program_1.c template------------------------------
*******************************************************************************/

#include <pthread.h> /* pthread functions and data structures for pipe */
#include <unistd.h>	 /* for POSIX API */
#include <stdlib.h>	 /* for exit() function */
#include <stdio.h>	 /* standard I/O routines */
#include <stdbool.h>
#include <string.h>
#include <semaphore.h>
#include <time.h>
#include <fcntl.h>
#include <errno.h>
#include <sys/mman.h>
#include <sys/types.h>
#include <sys/shm.h>
#include <sys/stat.h>

typedef struct
{
	int pid;
	int arrive_t, wait_t, burst_t, turnaround_t, remain_t;
} process;

#define PROCESSNUM 7
typedef struct RR_Params
{

	process processes[PROCESSNUM];
	int quantum_t;
	char outputFile[100];
	char *myfifo;
	int fd;
	float avg_wait_t;
	float avg_turnaround_t;
} ThreadParams;

int i;
pthread_t thread1, thread2;

void input_processes(ThreadParams *params)
{
	process *processes = params->processes;

	processes[0].pid = 1;
	processes[0].arrive_t = 8;
	processes[0].burst_t = 10;

	processes[1].pid = 2;
	processes[1].arrive_t = 10;
	processes[1].burst_t = 3;

	processes[2].pid = 3;
	processes[2].arrive_t = 14;
	processes[2].burst_t = 7;

	processes[3].pid = 4;
	processes[3].arrive_t = 9;
	processes[3].burst_t = 5;

	processes[4].pid = 5;
	processes[4].arrive_t = 16;
	processes[4].burst_t = 4;

	processes[5].pid = 6;
	processes[5].arrive_t = 21;
	processes[5].burst_t = 6;

	processes[6].pid = 7;
	processes[6].arrive_t = 26;
	processes[6].burst_t = 2;

	// Initialise remaining time to be same as burst time
	for (i = 0; i < PROCESSNUM; i++)
	{
		processes[i].remain_t = processes[i].burst_t;
	}
}

/* this function calculates Round Robin (RR) with a time quantum of 4, writes waiting time and turn-around time to the FIFO */
void *worker1(void *params)
{
	ThreadParams *tparams = (ThreadParams *)params;

	int time = 0, remain = 0;
	int done = 0;

	process *processes = tparams->processes;
	int quantum = tparams->quantum_t;

	while (remain != PROCESSNUM)
	{
		done = 1;
		for (i = 0; i < PROCESSNUM; i++)
		{
			if (processes[i].arrive_t <= time && processes[i].remain_t > 0)
			{
				done = 0;

				if (processes[i].remain_t > quantum)
				{
					time += quantum;
					processes[i].remain_t -= quantum;
				}
				else
				{
					time += processes[i].remain_t;
					processes[i].wait_t = time - processes[i].burst_t - processes[i].arrive_t;
					processes[i].turnaround_t = time - processes[i].arrive_t;

					tparams->avg_wait_t += processes[i].wait_t;
					tparams->avg_turnaround_t += processes[i].turnaround_t;

					processes[i].remain_t = 0;
					remain++;
				}
			}
		}

		if (done)
		{
			time++;
		}
	}
	
	params->avg_wait_t /= PROCESSNUM;
	params->avg_turnaround_t /= PROCESSNUM;
}

/* reads the waiting time and turn-around time through the FIFO and writes to text file */
void *worker2()
{
	// add your code here
}

/* this main function creates named pipe and threads */
int main(int argc, char *argv[])
{
	ThreadParams params;

	/* creating a named pipe(RR) with read/write permission */
	params.myfifo = "/tmp/myfifo";
	mkfifo(params.myfifo, 0666);
	params.fd = open(params.myfifo, O_RDWR);
	if (params.fd == -1)
	{
		perror("open FIFO for writing");
		exit(1);
	}

	/* initialize the parameters */
	input_processes((void *)(&params));
	params.avg_wait_t = 0.0;
	params.avg_turnaround_t = 0.0;

	/* Verify the correct number of arguments were passed in */
	if (argc != 3)
	{
		fprintf(stderr, "USAGE:./main.out 4 output.txt\n");
	}

	params.quantum_t = atoi(argv[1]);
	strcpy(params.outputFile, argv[2]);

	/* create threads */
	if (pthread_create(&thread1, NULL, worker1, NULL) != 0)
	{
		printf("Thread 1 created error\n");
		return -1;
	}
	if (pthread_create(&thread2, NULL, worker2, NULL) != 0)
	{
		printf("Thread 2 created error\n");
		return -2;
	}

	/* wait for the thread to exit */
	if (pthread_join(thread1, NULL) != 0)
	{
		printf("join thread 1 error\n");
		return -3;
	}
	if (pthread_join(thread2, NULL) != 0)
	{
		printf("join thread 2 error\n");
		return -4;
	}

	return 0;
}
