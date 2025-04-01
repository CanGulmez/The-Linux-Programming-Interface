/* Using a pipe to connect `ls` and `wc` */

#include "../linux.h"

void main(int argc, char *argv[])
{
   int pfd[2];             /* Pipe file descriptors */ 

   if (pipe(pfd) == -1)    /* Create pipe */
      syscall_error();

   switch (fork()) {
      case -1:
         syscall_error();

      case 0:              /* First child: exec 'ls' to write to pipe */
         if (close(pfd[0]) == -1)
            syscall_error();

         /* Duplicate stdout on write end of pipe; close duplicated descriptor */

         if (pfd[1] != STDOUT_FILENO) {
            if (dup2(pfd[1], STDOUT_FILENO) == -1)
               syscall_error();
            if (close(pfd[1]) == -1)
               syscall_error();
         }
         execlp("ls", "ls", (char *) NULL);
         syscall_error();
      
      default:
         break;
   }

   switch (fork()) {
      case -1:
         syscall_error();

      case 0:              /* Second child: exec 'wc' toread to pipe */
         if (close(pfd[1]) == -1)
            syscall_error();

         /* Duplicate stdin on read end of pipe; close duplicated descriptor */

         if (pfd[0] != STDIN_FILENO) {
            if (dup2(pfd[0], STDIN_FILENO) == -1)
               syscall_error();
            if (close(pfd[0]) == -1)
               syscall_error();
         }
         execlp("wc", "wc", "-l", (char *) NULL);
         syscall_error();
      
      default:
         break;
   }

   /* Parent closes unused file descriptors for pipe, and waits for children */
   
   if (close(pfd[0]) == -1)
      syscall_error();
   if (close(pfd[1]) == -1)
      syscall_error();
   if (wait(NULL) == -1)
      syscall_error();
   if (wait(NULL) == -1)
      syscall_error();

   exit(EXIT_SUCCESS);
}
