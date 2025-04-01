/* Retrieving process scheduling policies and priorities */

#include "../linux.h"

void main(int argc, char *argv[])
{
   int i, pol;
   struct sched_param sp;

   for (i = 1; i < argc; i++) {
      pol = sched_getscheduler(atol(argv[i]));
      if (pol == -1)
         syscall_error();

      if (sched_getparam(atol(argv[i]), &sp) == -1)
         syscall_error();

      printf("%s: %-5s %2d\n", argv[i],
         (pol == SCHED_OTHER) ? "OTHER" : 
         (pol == SCHED_RR) ? "RR" : 
         (pol == SCHED_FIFO) ? "FIFO" : 
#ifdef SCHED_BATCH
         (pol == SCHED_BATCH) ? "BATCH" : 
#endif 
#ifdef SCHED_IDLE
         (pol == SCHED_IDLE) ? "IDLE" : 
#endif
         "???", sp.sched_priority);
   }

   exit(EXIT_SUCCESS);
}
