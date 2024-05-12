/* Retrieving process CPU times */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <sys/times.h>
#include <assert.h>
#include <string.h>

/* Display 'msg' and process times */
static void displayProcessTimes(const char *msg)
{
   struct tms t;
   clock_t clockTimes;
   static long clockTicks = 0;

   if (msg != NULL)
      printf("%s", msg);

   if (clockTicks == 0)    // Fetch clock ticks on first call
   {
      clockTicks = sysconf(_SC_CLK_TCK);
      assert (clockTicks != -1);
   }

   clockTimes = clock();
   assert (clockTimes != -1);

   printf("    clock() returns: %ld clocks-per-second (%.2f secs)\n",
          (long) clockTimes, (double) clockTimes / CLOCKS_PER_SEC);

   assert (times(&t) != -1);

   printf("    times() yields: user CPU=%.2f; system CPU: %.2f\n", 
          (double) t.tms_utime / clockTicks, 
          (double) t.tms_stime / clockTicks);
}

void main(int argc, char *argv[])
{
   int numCalls;

   printf("CLOCKS_PER_SEC=%ld sysconf(_SC_CLK_TCK)=%ld\n\n", 
          (long) CLOCKS_PER_SEC, sysconf(_SC_CLK_TCK));

   displayProcessTimes("At time start:\n");

   numCalls = (argc > 1) ? atoi(argv[1]) : 100000000;

   for (int i=0; i<numCalls; i++)
      (void) getppid();

   displayProcessTimes("After getppif() loop:\n");

   exit(EXIT_SUCCESS);
}
