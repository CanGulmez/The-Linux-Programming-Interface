/* To temporarily prevent delivery of a signal, we can use the series 
   of calls shown in this program to block the signal, and then unlock
   it by resetting the signal mask to its previous state. 
*/

#include "./utils.h"

void main(int argc, char *argv[])
{
   sigset_t blockSet;

   /* Initialize a signal set to contain SIGINT */

   sigemptyset(&blockSet);
   sigaddset(&blockSet, SIGINT);

   /* Block SIGINT, save previous signal mask */

   if (sigprocmask(SIG_BLOCK, &blockSet, NULL) == -1)
      raiseError("sigprocmask()");
      
   /* ... Code that should not be interrupted by SIGINT ... */

   /* Restore previous signal mask, unblocking SIGINT */

   if (sigprocmask(SIG_UNBLOCK, &blockSet, NULL) == -1)
      raiseError("sigprocmask()");

   exit(EXIT_SUCCESS);
}
