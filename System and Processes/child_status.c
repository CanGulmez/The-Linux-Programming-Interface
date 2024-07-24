/* Using 'waitpid()' to retrieve the status of a child process */

#include "./utils.h"

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
             WSTOPSIG(status), atrsignal(WSTOPSIG(status)));
   else if (WIFCONTINUED(status))
      printf("child continued\n");
   else                             /* Should never hapen */
      printf("what happened to this child? (status=%x)\n",
             (unsigned int) status);
}

void main(int argc, char *argv[])
{
   int status;
   pid_t childPid;

   if (argc > 1 && !strcmp(argv[1], "--help"))
      raiseError("wrong command-line argument");

   switch (fork())
   {
      case -1:
         raiseError("fork()");

      case 0:           /* Child: either exits immediately with given 
                           status or loops waiting for signals */
         printf("Child started with PID = %ld\n", (long) getpid());
         if (argc > 1)           /* Status supplied on command line? */
            exit(atoi(argv[1]));
         else                    /* Otherwise, wait for signals */
            for (;;)
               pause();
         exit(EXIT_SUCCESS);     /* No reached, but good practice */

      default:          /* Parent: repeatedly wait on child until it
                           either exits or is terminated by signal */
         for (;;)
         {
            childPid = waitpid(-1, &status, WUNTRACED | WCONTINUED);

            if (childPid == -1)
               raiseError("waitpid()");
            
            /* Print status in hex, and as seperate decimal bytes */

            printf("waitpid() returned: PID=%ld, status=0%04x (%d, %d)\n",
                   (long) childPid,
                   (unsigned int) status, status >> 8, status & 0xff);
            printWaitStatus(NULL, status);

            if (WIFEXITED(status) || WIFSIGNALED(status))
               exit(EXIT_SUCCESS);
         } 
   }
}

