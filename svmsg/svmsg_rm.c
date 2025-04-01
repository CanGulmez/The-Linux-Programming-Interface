/* Deleting System V message queues */

#include "../linux.h"

void main(int argc, char *argv[])
{
   int i;

   if (argc > 1 && strcmp(argv[1], "--help") == 0)
      usage_error("Wrong command-line usage");

   for (i = 1; i < argc; i++) 
      if (msgctl(atoi(argv[i]), IPC_RMID, NULL) == -1)
         syscall_error();

   exit(EXIT_SUCCESS);
}
