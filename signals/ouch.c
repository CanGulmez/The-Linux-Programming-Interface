/* Installing a handler for SIGINT */

#include "../linux.h"

void sigHandler(int sig)
{
   printf("Ouch!\n");
}

void main(int argc, char *argv[])
{
   int i;

   if (signal(SIGINT, sigHandler) == SIG_ERR)
      syscall_error();

   for (i = 0; ; i++) {
      printf("%d\n", i);
      sleep(1);
   }

   exit(EXIT_SUCCESS);
}
