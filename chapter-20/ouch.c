/* Installing a handler for SIGINT */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <signal.h>

static void sigHandler(int sig)   
{
   printf("Ouch!\n");            // UNSAFE (see Section 21.1.2)
}

void main(int argc, char *argv[])
{
   assert (signal(SIGINT, sigHandler) != SIG_ERR);

   for (int i=0; ; i++)
   {
      printf("%d\n", i);
      sleep(3);                  // Loop slowly...
   }
}
