/* Modifying process scheduling policies and priorities */

#include "../linux.h"

void main(int argc, char *argv[])
{
   int i, pol;
   struct sched_param sp;

   if (argc < 3 || strchr("rfo", argv[1][0]) == NULL)
      usage_error("Wrong command line usage");

   pol = (argv[1][0] == 'r') ? SCHED_RR :
         (argv[1][0] == 'f') ? SCHED_FIFO :
#ifdef SCHED_BATCH
         (argv[1][0] == 'b') ? SCHED_BATCH :
#endif
#ifdef SCHED_IDLE
         (argv[1][0] == 'i') ? SCHED_IDLE :
#endif
         SCHED_OTHER;

   sp.sched_priority = atoi(argv[2]);

   for (i = 3; i < argc; i++)
      if (sched_setscheduler(atol(argv[i]), pol, &sp) == -1)
         syscall_error();

   exit(EXIT_SUCCESS);
}
