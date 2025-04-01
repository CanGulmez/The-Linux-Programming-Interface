/* Executing shell commands with 'system()' */

#include "../linux.h"

#define MAX_CMD_LEN     200

void printWaitStatus(const char *msg, int status)
{
   if (msg != NULL)
      printf("%s", msg);

   if (WIFEXITED(status)) {
      printf("child exited, status=%d\n", WEXITSTATUS(status));
   } else if (WIFSIGNALED(status)) {
      printf("child killed by signal %d (%s)", 
         WTERMSIG(status), strsignal(WTERMSIG(status)));
#ifdef WCOREDUMP
   if (WCOREDUMP(status))
      printf(" (core dumped)");
#endif
      printf("\n");
   } else if (WIFSTOPPED(status)) {
      printf("child stopped by signal %d (%s)\n",
         WSTOPSIG(status), strsignal(WSTOPSIG(status)));
#ifdef WIFCONTINUED
   } else if (WIFCONTINUED(status)) {
      printf("child continued\n");
#endif
   } else {
      printf("what happened to this child? (status=%x)\n",
         (unsigned int) status);
   }
}

void main(int argc, char *argv[])
{
   char str[MAX_CMD_LEN];     /* Command to be executed by system() */
   int status;                /* Status return from system() */

   for (;;) {                 /* Read and execute a shell command */
      printf("Command: ");
      fflush(stdout);
      if (fgets(str, MAX_CMD_LEN, stdin) == NULL)
         break;               /* end-of-file */

      status = system(str);
      printf("system() returned: status=0x%04x (%d,%d)\n",
         (unsigned int) status, status >> 8, status & 0xff);

      if (status == -1)
         syscall_error();
      else {
         if (WIFEXITED(status) && WEXITSTATUS(status) == 127)
            printf("(Probably) could not invoke shell\n");
         else                 /* Shell successfully executed command */
            printWaitStatus(NULL, status);
      }
   }
   exit(EXIT_SUCCESS);
}
