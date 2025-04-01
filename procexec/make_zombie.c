/* Creating a zombie child process */

#include "../linux.h"

#define CMD_SIZE     200

void main(int argc, char *argv[])
{
   char cmd[CMD_SIZE];
   pid_t childPid;

   setbuf(stdout, NULL);      /* Disable buffering of stdout */

   printf("Parent PID=%ld\n", (long) getpid());

   switch (childPid = fork()) {
      case -1:
         syscall_error();
      case 0:     /* Child: immediately exits to become zombie */
         printf("Child (PID=%ld) exiting\n", (long) getpid());
         _exit(EXIT_SUCCESS);
      default:    /* Parent */
         sleep(3);            /* Give child a change to start and exit */
         snprintf(cmd, CMD_SIZE, "ps | grep %s", basename(argv[0]));
         cmd[CMD_SIZE - 1] = '\0';
         system(cmd);         /* View zombie child */

         /* Now send the "sure kill" signal to the zombie */

         if (kill(childPid, SIGKILL) == -1)
            syscall_error();

         sleep(3);            /* Give child a change to react to signal */
         printf("After sending SIGKILL to zombie (PID=%ld):\n", (long) childPid);
         system(cmd);         /* View zombie child again */

         exit(EXIT_SUCCESS);
   }
}
