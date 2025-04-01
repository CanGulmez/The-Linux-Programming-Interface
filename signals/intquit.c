/* Establishing the same handler for two diffenrent signals */

#include "../linux.h"

void sigHandler(int sig)
{
   static int count;

   if (sig == SIGINT) {
      count ++;
      printf("Caught SIGINT (%d)\n", count);
      return;
   }

   /* Must be SIGQUIT - print a message and terminate the process */

   printf("Caught SIGQUIT - that's all forks!\n");
   exit(EXIT_SUCCESS);
}

void main(int argc, char *argv[])
{
   /* Establish same handler for SIGINT and SIGQUIT */

   if (signal(SIGINT, sigHandler) == SIG_ERR)
      syscall_error();
   if (signal(SIGQUIT, sigHandler) == SIG_ERR)
      syscall_error();

   for (;;)
      pause();
}
