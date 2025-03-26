/*
----- 48450 -- week 6 lab ------ 
By programing the file, you will follow the lecture notes to implement the clock set up.

This program sets the clock forward 1 day. 

*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>

int main( void )
  {
    struct timespec stime;
    time_t now;

	/* get the current system time*/
    if( clock_gettime( CLOCK_REALTIME, &stime) == -1 ) {
       perror( "getclock" );
       exit( EXIT_FAILURE );
    }
    
    time(&now);/* to get current system time as structure*/
    printf("Current time is: %s\n", ctime(&now));//ctime: Convert Date and Time to String)
	
    stime.tv_sec += (60*60)*12L;  /* Add one day */
    stime.tv_nsec = 0;
	/*set the system time into a new time*/
    if( clock_settime( CLOCK_REALTIME, &stime) == -1 ) {
       perror( "setclock" );
       exit( EXIT_FAILURE );
    }

   time(&now);
    printf("After changing the time, current time is: %s\n", ctime(&now));
    return( EXIT_SUCCESS );
  }

