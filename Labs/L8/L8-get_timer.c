/*
 * This program calculates the time required to
 * execute the program specified as its first argument.
 * The time is printed in seconds, on standard out.
 */
#include <stdio.h>
#include <unistd.h>
#include <stdlib.h>
#include <time.h>

#define BILLION  1E9

int main(void)
  {
    struct timespec start, stop, stime;
    double accum;
	int clockReturn;
	
	/* get the start time*/
	//add your program here
  if( clock_gettime( CLOCK_REALTIME, &start) == -1 ) {
    perror( "getclock" );
    exit( EXIT_FAILURE );
 }
 time_t now;
 time(&now);
 printf("Current time is: %s\n", ctime(&now));

    if(clockReturn  == -1 ) {
      perror( "clock gettime start" );
      exit( EXIT_FAILURE );
    }
	
	/* program caculation*/
    int i, sum=0;
    for(i=0;i<10;i++){
		sum+=i;
	sleep(1);}

printf("Sum=%d\n", sum);

	/* get the end time*/
	//add your program here
	
if( clock_gettime( CLOCK_REALTIME, &stop) == -1 ) {
    perror( "getclock" );
    exit( EXIT_FAILURE );
 }

 time(&now);
 
 printf("Current time is: %s\n", ctime(&now));
    if( clockReturn == -1 ) {
      perror( "clock gettime stop" );
      exit( EXIT_FAILURE );
    }

    /*second + nano second*/
    accum = ( stop.tv_sec - start.tv_sec )
          + ( stop.tv_nsec - start.tv_nsec )/ BILLION;
    printf( "%lf second\n", accum );
    return( EXIT_SUCCESS );
  }
