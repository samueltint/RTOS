#include <stdio.h>
#include <stdlib.h>
#include <pthread.h>

void *functionC(void *param);
pthread_mutex_t mutex1 = PTHREAD_MUTEX_INITIALIZER;
int  counter = 0;

int main()
{
   int rc1, rc2;
   int rc1_No, rc2_No;
   pthread_t thread1, thread2;

   /* Create independent threads each of which will execute functionC */

	rc1_No=1; 
   if( (rc1=pthread_create( &thread1, NULL, &functionC, &rc1_No)) )
   {
      printf("Thread creation failed: %d\n", rc1);
   }
   
	rc2_No=2;
   if( (rc2=pthread_create( &thread2, NULL, &functionC, &rc2_No)) )
   {
      printf("Thread creation failed: %d\n", rc2);
   }

   /* Wait till threads are complete before main continues. Unless we  */
   /* wait we run the risk of executing an exit which will terminate   */
   /* the process and all threads before the threads have completed.   */
   pthread_join( thread1, NULL);
   pthread_join( thread2, NULL); 
 
   exit(EXIT_SUCCESS);
   
}

void *functionC(void *param)
{
   /* lock the memory by mutex */
   
   pthread_mutex_lock( &mutex1 );
   int *fd = (int*)param;
   printf("rc Number=%d\n", *fd);
   int i=0;
   for(i=0;i<10000;i++)
       counter++;
   printf("Counter value: %d\n",counter);
   /* unlock the memory by mutex */
   
   pthread_mutex_unlock( &mutex1 );
}
