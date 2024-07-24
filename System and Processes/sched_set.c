/* Modifying process scheduling policies and priorities 

   This program uses 'sched_setscheduler()' to set the policy and priority
   of the processes specified by its command-line arguments. THe first
   argument is a letter specifying a scheduling policy, the second is an
   imteger priority, and the remaining arguments are the prcess IDs of 
   the processes whose scheduling attributes are to be changed.
*/

#include "./utils.h"

void main(int argc, char *argv[])
{
   int i, pol;
   struct sched_param sp;

   if (argc < 3 || strchr("rfo", argv[1][0]) == NULL)
      raiseError("wrong command-line aguments");

   pol = (argv[1][0] == 'r') ? SCHED_RR : 
         (argv[1][0] == 'f') ? SCHED_FIFO :
         (argv[1][0] == 'b') ? SCHED_BATCH :
         (argv[1][0] == 'i') ? SCHED_IDLE :
                               SCHED_OTHER;

   sp.sched_priority = atoi(argv[2]); 

   for (i=3; i<argc; i++)
      if (sched_setscheduler(atol(argv[i]), pol, &sp) == -1)
         raiseError("sched_setscheduler()");

   exit(EXIT_SUCCESS);
}
