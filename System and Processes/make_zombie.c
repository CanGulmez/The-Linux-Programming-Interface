/* Creating a zombie child process */

#include "./utils.h"

#define CMD_SIZE     200

void main(int argc, char *argv[])
{
   char command[CMD_SIZE];
   pid_t childPid;

   setbuf(stdout, NULL);         /* Disable buffering of stdout */

   printf("Parent PID=%ld\n", (long) getpid());

   switch (childPid = fork())
   {
      case -1:
         raiseError("fork()"); 

      case 0:        /* Child: immediately exits to become zombie */
         printf("Child (PID=%ld) exiting\n", (long) getpid());
         _exit(EXIT_SUCCESS);

      default:       /* Parent */
         sleep(3);          /* Give child a change to start and exit */
         snprintf(command, CMD_SIZE, "ps | grep %s", basename(argv[0]));
         command[CMD_SIZE - 1] = '\0'; /* Ensure string is null-terminated */
         system(command);     /* View zombie child */

         /* Now send the "sure KILL" signal to the zombie */

         if (kill(childPid, SIGKILL) == -1)
            raiseError("kill()");

         sleep(3);            /* Give child a change to react to signal */
         printf("After sending SIGKILL to zombie (PID=%ld):\n", (long) childPid);
         system(command);     /* View zombie child again */

         exit(EXIT_SUCCESS);
   }      
}
