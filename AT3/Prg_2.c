/*******************************************************************************
The assignment 3 for subject 48450 (RTOS) in University of Technology Sydney(UTS)
This is a template of Program_2.c template. Please complete the code based on
the assignment 3 requirement. Assignment 3

------------------------------Program_2.c template------------------------------
*******************************************************************************/

#include <stdio.h>
#include <pthread.h>
#include <string.h>
#include <stdbool.h>
#include <stdlib.h>
#include <unistd.h>
#include <semaphore.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <signal.h>

// Number of pagefaults in the program
int pageFaults = 0;

// Function declaration
void SignalHandler(int signal);

/**
 Main routine for the program. In charge of setting up threads and the FIFO.

 @param argc Number of arguments passed to the program.
 @param argv array of values passed to the program.
 @return returns 0 upon completion.
 */
int main(int argc, char *argv[])
{
  // Register Ctrl+c(SIGINT) signal and call the signal handler for the function.
  signal(SIGINT, SignalHandler);

  if (argc != 2)
  {
    fprintf(stderr, "USAGE:./main.out 4\n");
  }

  int i;
  // reference number
  int REFERENCESTRINGLENGTH = 24;
  // Argument from the user on the frame size, such as 4 frames in the document
  int frameSize = atoi(argv[1]);
  // Frame where we will be storing the references. -1 is equivalent to an empty value
  uint frame[REFERENCESTRINGLENGTH];
  // Reference string from the assignment outline
  int referenceString[24] = {7, 0, 1, 2, 0, 3, 0, 4, 2, 3, 0, 3, 0, 3, 2, 1, 2, 0, 1, 7, 0, 1, 7, 5};
  // Next position to write a new value to.
  int nextWritePosition = 0;
  // Boolean value for whether there is a match or not.
  bool match = false;
  // Current value of the reference string.
  int currentValue;

  // Initialise the empty frame with -1 to simulate empty values.
  for (i = 0; i < frameSize; i++)
  {
    frame[i] = -1;
  }

  for (i = 0; i < REFERENCESTRINGLENGTH; i++)
  {
    match = false;
    currentValue = referenceString[i];

    for (int j = 0; j < frameSize; j++)
    {
      if (currentValue == frame[j])
      {
        match = true;
        pageFaults--;
      }
    }
    pageFaults++;

    if ((pageFaults <= frameSize) && (!match))
    {
      frame[i] = currentValue;
    }
    else if (!match)
    {
      frame[(pageFaults - 1) % frameSize] = currentValue;
      printf("Page Fault %d\n", pageFaults);

    }

    printf("%d\t\t", referenceString[i]); 
    for (int n = 0; n < frameSize; n++)   
    {
      if (frame[n] != -1)            
        printf(" %d\t\t", frame[n]); 
      else
        printf(" - \t\t"); 
    }
    printf("\n");
  }

  // Sit here until the ctrl+c signal is given by the user.
  while (1)
  {
    sleep(1);
  }

  return 0;
}

/**
 Performs the final print when the signal is received by the program.

 @param signal An integer values for the signal passed to the function.
 */
void SignalHandler(int signal)
{

  // add your code
  printf("\nTotal page faults: %d\n", pageFaults);
  exit(0);
}
