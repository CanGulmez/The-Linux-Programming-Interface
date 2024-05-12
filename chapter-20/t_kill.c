/* Using the kill() system call */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <errno.h>
#include <string.h>
#include <signal.h>

void main(int argc, char *argv[])
{
   int s, sig;

   assert (argc == 3 && strcmp(argv[1], "--help"));

   sig = atoi(argv[2]);
   s = kill(atol(argv[1]), sig);

   if (sig != 0) 
   {
      assert (s != -1);
   }
   else           // NULL signal: process existence check
   {
      if (s == 0)
      {
         printf("Process exists and we can send it a signal\n");
      }
      else
      {
         if (errno == EPERM)
            printf("Process exists, but we don't have "
                   "permission to send it a signal\n");
         else if (errno == ESRCH)
            printf("Process does not exist\n");
         else
            assert (0);
      } 
   }
   exit(EXIT_SUCCESS);
}
 