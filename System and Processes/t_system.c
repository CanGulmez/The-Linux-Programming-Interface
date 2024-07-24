/* Executing shell commands with 'system()' 

   The program demonstrates the use of 'system()'. This program executes
   a loop that reads a command string, executes it using 'system()', and
   then analyzes and displays the value returned by 'system()'.
*/

#include "./utils.h"

#define MAX_CMD_LEN 200

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

void main(int argc, char *argv[])
{
   char command[MAX_CMD_LEN];    /* Command to be executed by system() */
   int status;                   /* Status return from system() */

   for (;;)                      /* Read and execute a shell command */
   {
      printf("Command: ");
      fflush(stdout);
      if (fgets(command, MAX_CMD_LEN, stdin) == NULL)
         break;         /* end-of-file */

      status = system(command);
      printf("system() returned: status=0%04x (%d, %d)\n", 
             (unsigned int) status, status >> 8, status & 0xff);

      if (status == -1)
         raiseError("system()");
      else
      {
         if (WIFEXITED(status) && WEXITSTATUS(status) == 127)
            printf("(Probably) could not invoke shell\n");
         else                /* Shell successfully executed command */
            printWaitStatus(NULL, status); 
      }
   }
   exit(EXIT_SUCCESS);
}
