/***********************************************************************************/
//***********************************************************************************
//            *************NOTE**************
// This is a template for the subject of RTOS in University of Technology Sydney(UTS)
// Please complete the code based on the assignment requirement.

//***********************************************************************************
/***********************************************************************************/

/*
  To compile assign2_template-v3.c ensure that gcc is installed and run
  the following command:

  gcc your_program.c -o your_ass-2 -lpthread -lrt -Wall
*/

#include <pthread.h>
#include <stdlib.h>
#include <unistd.h>
#include <stdio.h>
#include <sys/types.h>
#include <fcntl.h>
#include <string.h>
#include <sys/stat.h>
#include <semaphore.h>
#include <sys/time.h>

/* to be used for your memory allocation, write/read. man mmsp */
#define SHARED_MEM_NAME "/my_shared_memory"
#define SHARED_MEM_SIZE 1024

/* --- Structs --- */
typedef struct ThreadParams
{
  int pipeFile[2];           // [0] for read and [1] for write. use pipe for data transfer from thread A to thread B
  sem_t sem_A, sem_B, sem_C; // the semphore
  char message[255];
  char inputFile[100];  // input file name
  char outputFile[100]; // output file name
} ThreadParams;

/* Global variables */
int sum = 1;

pthread_attr_t attr;

int shm_fd; // use shared memory for data transfer from thread B to Thread C

/* --- Prototypes --- */

/* Initializes data and utilities used in thread params */
void initializeData(ThreadParams *params);

/* This thread reads data from data.txt and writes each line to a pipe */
void *ThreadA(void *params);

/* This thread reads data from pipe used in ThreadA and writes it to a shared variable */
void *ThreadB(void *params);

/* This thread reads from shared variable and outputs non-header text to src.txt */
void *ThreadC(void *params);

/* --- Main Code --- */
int main(int argc, char const *argv[])
{

  pthread_t tid[3]; // three threads
  ThreadParams params;

  /* Verify the correct number of arguments were passed in */
  if (argc != 3)
  {
    fprintf(stderr, "USAGE:./main.out data.txt output.txt\n");
  }

  strcpy(params.inputFile, argv[1]);
  strcpy(params.outputFile, argv[2]);

  // Initialization
  initializeData(&params);

  // Create Threads
  pthread_create(&(tid[0]), &attr, &ThreadA, (void *)(&params));

  // TODO: add your code
  pthread_create(&(tid[1]), &attr, &ThreadB, (void *)(&params));
  pthread_create(&(tid[2]), &attr, &ThreadC, (void *)(&params));

  // Wait on threads to finish
  pthread_join(tid[0], NULL);

  // TODO: add your code
  pthread_join(tid[1], NULL);
  pthread_join(tid[2], NULL);


  return 0;
}

void initializeData(ThreadParams *params)
{
  int result;
  
  // Initialize Sempahores
  if (sem_init(&(params->sem_A), 0, 1) != 0)
  { // Set up Sem for thread A
    perror("error for init threa A");
    exit(1);
  }
  if (sem_init(&(params->sem_B), 0, 0) != 0)
  { // Set up Sem for thread B
    perror("error for init threa B");
    exit(1);
  }
  if (sem_init(&(params->sem_C), 0, 0) != 0)
  { // Set up Sem for thread C
    perror("error for init threa C");
    exit(1);
  }

  // Initialize thread attributes
  pthread_attr_init(&attr);

  // TODO: add your code

  result = pipe(params->pipeFile);
  if (result < 0)
  {
    perror("pipe error");
    exit(1);
  }


  return;
}

void *ThreadA(void *params)
{
  // TODO: add your code
  ThreadParams *tparams = (ThreadParams*)params;
  char c[1000];
  FILE *fptr;
  int sig;
  char check[1000] = "end_header";
  int result;

  if ((fptr = fopen(tparams->inputFile, "r")) == NULL)
  {
    printf("Error! opening file");
    // Program exits if file pointer returns NULL.
    exit(1);
  }

  // reads text until newline is encountered
  printf("reading from the file:\n");

  sig = 0;

  while (fgets(c, sizeof(c), fptr) != NULL)
  {
    result= write(tparams->pipeFile[1], c, 1);
    if (result!=1){ 
      perror ("failed to write to pipe"); 
      exit (2);
	  }
  }

  fclose(fptr);

  printf("Thread A: sum = %d\n", sum);
}

void *ThreadB(void *params)
{  
  
  ThreadParams *tparams = (ThreadParams*)params;
  buffer_item item[100];
  int result;

  printf ("In reading thread\n");
  while(1){
    char c;

    result = read(tparams->pipeFile[0], item, sizeof(item));

    
    if(c !='\0')	{
      printf ("Reader: %c\n", c);
    } else {
      printf("reading pipe has completed\n");
      exit (5);
    }
  }

  printf("Thread B: sum = %d\n", sum);
}

void *ThreadC(void *params)
{
  // TODO: add your code

  printf("Thread C: Final sum = %d\n", sum);
}
