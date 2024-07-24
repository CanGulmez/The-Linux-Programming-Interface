/* Retrieving process scheduling policies and priorities 

   The program uses 'sched_getscheduler()' and 'sched_getparam()' to 
   retriev the policy and priority of all the processes whose IDs are
   given as command-line arguments.
*/

#include "./utils.h"

void main(int argc, char *argv[])
{
   int i, pol;
   struct sched_param sp;

   for (i=1; i<argc; i++)
   {
      pol = sched_getscheduler(atoi(argv[i]));
      if (pol == -1)
         raiseError("sched_getscheduler()");

      if (sched_getparam(atol(argv[i]), &sp) == -1)
         raiseError("sched_getparam()");

      printf("%s: %-5s %2d\n", argv[i], 
             (pol == SCHED_OTHER) ? "OTHER" : 
             (pol == SCHED_RR) ? "RR" : 
             (pol == SCHED_FIFO) ? "FIFO" :
             (pol == SCHED_BATCH) ? "BATCH" : 
             (pol == SCHED_IDLE) ? "IDLE" : "???", sp.sched_priority);
   }

   exit(EXIT_SUCCESS);
}
