/* Installing a handler for SIGINT */

#include "./utils.h"

void sigHandler(int sig)
{
   printf("Signal catched!\n");     /* UNSAFE (see Section 21.1.2) */
   sleep(5);
}

void main(int argc, char *argv[])
{
   int i;

   if (signal(SIGINT, sigHandler) == SIG_ERR)
      raiseError("signal()");

   for (i=0; ; i++)
   {
      printf("%d\n", i);
      sleep(1);                     /* Loop slowly... */
   }
}
