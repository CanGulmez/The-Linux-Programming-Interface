/* Modifying and retrieving a process's nice value */

#include "../linux.h"

void main(int argc, char *argv[])
{
   int which, prio;
   id_t who;

   if (argc != 4 || strchr("pgu", argv[1][0]) == NULL)
      usage_error("Wrong command-line usage");

   /* Set nice value according to command-line arguments */

   which = (argv[1][0] == 'p') ? PRIO_PROCESS : 
      (argv[1][0] == 'g') ? PRIO_PGRP : PRIO_USER;
   who = atol(argv[2]);
   prio = atoi(argv[3]);

   if (setpriority(which, who, prio) == -1)
      syscall_error();

   /* Retrieve nice value to check the change */

   errno = 0;        /* Because successful call may return -1 */
   prio = getpriority(which, who);
   if (prio == -1 && errno != 0)
      syscall_error();

   printf("Nice value = %d\n", prio);

   exit(EXIT_SUCCESS);
}
