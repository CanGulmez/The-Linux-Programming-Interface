/* Displaying the status value returned by 'wait()' and related calls */

#include "./utils.h"

/* NOTE: The following function employs printf(), which is not
   async-signal-safe (see Section 21.1.2). As such, this function is
   also not async-signal-safe (i.e., beware of calling it from a
   SIGCHLD handler). */

/* Examine a wait() status using the W* macros */
void printWaitStatus(const char *msg, int status)
{
   if (msg != NULL)
      printf("%s", msg);

   if (WIFEXITED(status))
      printf("child exited, status=%d\n", WEXITSTATUS(status));
   else if (WIFSIGNALED(status)) 
   {
      printf("child killed by signal %d (%s)", 
             WTERMSIG(status), strsignal(WTERMSIG(status)));

      if (WCOREDUMP(status))
         printf(" (coredump)");
      printf("\n");
   }
   else if (WIFSTOPPED(status))
      printf("child stopped by signal %d (%s)\n", 
             WSTOPSIG(status), strsignal(WSTOPSIG(status)));
   else if (WIFCONTINUED(status))
      printf("child continued\n");
   else                             /* Should never hapen */
      printf("what happened to this child? (status=%x)\n",
             (unsigned int) status);
}
