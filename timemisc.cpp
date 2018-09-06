#include "timemisc.h"

unsigned int timeval_subtract (struct timeval *result, struct timeval *x, struct timeval *y)
     {
       /* Perform the carry for the later subtraction by updating y. */
       if (x->tv_usec < y->tv_usec) {
         unsigned int nsec = (y->tv_usec - x->tv_usec) / 1000000 + 1;
         y->tv_usec -= 1000000 * nsec;
         y->tv_sec += nsec;
       }
       if (x->tv_usec - y->tv_usec > 1000000) 
	{
         unsigned int nsec = (x->tv_usec - y->tv_usec) / 1000000;
         y->tv_usec += 1000000 * nsec;
         y->tv_sec -= nsec;
       }
     
       /* Compute the time remaining to wait.
          tv_usec is certainly positive. */
       result->tv_sec = x->tv_sec - y->tv_sec;
       result->tv_usec = x->tv_usec - y->tv_usec;
     
       /* Return 1 if result is negative. */
       return x->tv_sec < y->tv_sec;
     }

/**
 * Print the time beetween the start and the end of the execution
 */
double get_time(struct timeval *start, struct timeval *end)
{
	//struct timeval result;
        double execution_time=0;  // gets the execution time

	struct timeval result;
	timeval_subtract(&result, end, start);

	execution_time = (result.tv_sec*1000000.0 + result.tv_usec)/1000000.0;
        return execution_time;
}

