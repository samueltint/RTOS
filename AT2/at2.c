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
#include <sys/mman.h>
#include <stdbool.h>

/* to be used for your memory allocation, write/read. man mmsp */
#define SHARED_MEM_NAME "/my_shared_memory"
#define SHARED_MEM_SIZE 1024

/* --- Structs --- */
typedef struct ThreadParams
{
  int pipeFile[2];           // [0] for read and [1] for write. use pipe for data transfer from thread A to thread B
  // sem_t sem_A, sem_B, sem_C; // the semphore
  char message[255];
  char inputFile[100];  // input file name
  char outputFile[100]; // output file name
} ThreadParams;

/* Global variables */
char eof_marker[4] = "EOF\n";

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
  if (pthread_create(&(tid[0]), &attr, &ThreadA, (void *)(&params)) != 0)
  {
    perror("failed create thread A\n");
    exit(1);
  }
  if (pthread_create(&(tid[1]), &attr, &ThreadB, (void *)(&params)) != 0)
  {
    perror("failed create thread B\n");
    exit(1);
  }
  if (pthread_create(&(tid[2]), &attr, &ThreadC, (void *)(&params)) != 0)
  {
    perror("failed create thread C\n");
    exit(1);
  }

  // Wait on threads to finish
  pthread_join(tid[0], NULL);
  pthread_join(tid[1], NULL);
  pthread_join(tid[2], NULL);

  return 0;
}

void initializeData(ThreadParams *params)
{
  int result;

  // if (sem_init(&(params->sem_A), 0, 1) != 0)
  // {
  //   perror("error for init threa A");
  //   exit(1);
  // }
  // if (sem_init(&(params->sem_B), 0, 0) != 0)
  // { 
  //   perror("error for init threa B");
  //   exit(1);
  // }
  // if (sem_init(&(params->sem_C), 0, 0) != 0)
  // { 
  //   perror("error for init threa C");
  //   exit(1);
  // }

  pthread_attr_init(&attr);

  shm_fd = shm_open(SHARED_MEM_NAME, O_CREAT | O_RDWR, 0666);
  ftruncate(shm_fd, SHARED_MEM_SIZE);

  result = pipe(params->pipeFile);
  if (result < 0)
  {
    perror("pipe error");
    exit(1);
  }

  return;
}
// Inside ThreadA
void *ThreadA(void *params)
{
  ThreadParams *tparams = (ThreadParams *)params;
  char item[1000];
  FILE *fptr;
  int result;

  if ((fptr = fopen(tparams->inputFile, "r")) == NULL)
  {
    printf("Error! opening input file\n");
    exit(1);
  }

  while (fgets(item, sizeof(item), fptr) != NULL)
  {
    // sem_wait(&(tparams->sem_A));
    result = write(tparams->pipeFile[1], item, strlen(item));
    printf("[Thread A] Read from file and wrote to pipe: %s", item);
    if (result <= 0)
    {
      perror("failed to write to pipe");
      exit(2);
    }
    // sem_post(&(tparams->sem_B));
  }

  // sem_wait(&(tparams->sem_A));
  write(tparams->pipeFile[1], eof_marker, strlen(eof_marker));
  printf("[Thread A] Wrote EOF to pipe\n");
  // sem_post(&(tparams->sem_B));

  fclose(fptr);
}

// Inside ThreadB
void *ThreadB(void *params)
{
  ThreadParams *tparams = (ThreadParams *)params;
  char item[1000];
  int bytesRead;
  void *memptr;

  memptr = mmap(0, SHARED_MEM_SIZE, PROT_WRITE, MAP_SHARED, shm_fd, 0);
  if (memptr == MAP_FAILED)
  {
    perror("mmap failed in Thread B");
    exit(1);
  }

  while (1)
  {
    // sem_wait(&(tparams->sem_B));
    bytesRead = read(tparams->pipeFile[0], item, sizeof(item) - 1);
    item[bytesRead] = '\0';

    printf("[Thread B] Read from pipe: %s", item);
    sprintf(memptr, "%s", item);
    printf("[Thread B] Wrote to shared memory: %s", (char *)memptr);

    // sem_post(&(tparams->sem_C));

    if (strcmp(item, eof_marker) == 0)
    {
      break;
    }
  }
}

// Inside ThreadC
void *ThreadC(void *params)
{
  ThreadParams *tparams = (ThreadParams *)params;
  void *memptr;
  char item[1000];
  bool reading_content = false;
  FILE *fptr;

  memptr = mmap(0, SHARED_MEM_SIZE, PROT_READ, MAP_SHARED, shm_fd, 0);
  if (memptr == MAP_FAILED)
  {
    perror("mmap failed in Thread C");
    exit(1);
  }

  if ((fptr = fopen(tparams->outputFile, "w")) == NULL)
  {
    printf("Error! opening output file\n");
    exit(1);
  }

  while (1)
  {
    // sem_wait(&(tparams->sem_C));
    strcpy(item, (char *)memptr);

    printf("[Thread C] Read from shared memory: %s", item);

    if (strcmp(item, eof_marker) == 0)
    {
      printf("[Thread C] Received EOF marker. Exiting.\n");
      break;
    }

    char *start = item;
    char *end;

    while ((end = strchr(start, '\n')) != NULL)
    {
      *end = '\0';
      if (reading_content)
      {
        printf("[Thread C] Writing line to output: %s\n", start);
        fprintf(fptr, "%s\n", start);
      }
      else
      {
        if (strcmp(start, "end_header") == 0)
        {
          reading_content = true;
          printf("[Thread C] Found end_header. Now writing content.\n");
        }
        else
        {
          printf("[Thread C] Skipping header line: %s\n", start);
        }
      }
      start = end + 1;
    }
    // sem_post(&(tparams->sem_A));
  }

  fclose(fptr);
  return NULL;
}
