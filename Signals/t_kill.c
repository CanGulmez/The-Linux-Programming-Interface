/* Using the 'kill()' system call */

#include "./utils.h"

void main(int argc, char *argv[])
{
   int s, sig; 

   if (argc != 3 || !strcmp(argv[1], "--help"))
      raiseError("wrong command-line arguments");

   sig = atoi(argv[2]);

   s = kill(atol(argv[1]), sig); 

   if (sig != 0)
   {
      if (s == -1)
         raiseError("kill()");
   }
   else                  /* Null signal: process existance check */
   {
      if (s == 0)
         printf("Process exists and we can send it a signal\n");
      else
      {
         if (errno == EPERM)
            printf("Process exists, but we don't have permission "
                   "to send it a signal\n");
         else if (errno == ESRCH)
            printf("Process does not exist\n");
         else
            raiseError("kill()");
      }
   }

   exit(EXIT_SUCCESS);
}
