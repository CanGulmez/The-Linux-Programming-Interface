/* Using the 'kill()' system call */

#include "../linux.h"

void main(int argc, char *argv[])
{
   int s, sig;

   if (argc != 3 || strcmp(argv[1], "--help") == 0)
      usage_error("Wrong command-line usage");

   sig = atoi(argv[2]);
   s = kill(atol(argv[1]), sig); 
   
   if (sig != 0) {
      if (s == -1)
         syscall_error();
   } else {    /* Null signal: process existence check */
      if (s == 0) {
         printf("Process exists and we can send it a signal\n");
      } else {
         if (errno == EPERM)
            printf("Process exists, but we don't have "
                   "permission to send it a signal\n");
         else if (errno == ESRCH)
               printf("Process doesn't exist\n");
         else 
            syscall_error();
      }
   }
   exit(EXIT_SUCCESS);
}
