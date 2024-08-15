/* Using a pipe to connect 'ls' and 'wc' */

#include "./utils.h"

void main(int argc, char *argv[])
{
   int pfd[2];                      /* Pipe file descriptors */

   if (pipe(pfd) == -1)             /* Create pipe */
      raiseError("pipe()");

   switch (fork()) {

      case -1:
         raiseError("fork()");

      case 0:           /* First child: exec 'ls' to write to pipe */
         if (close(pfd[0]) == -1)      /* Read end is unused */
            raiseError("close()");

         /* Duplicate stdout on write end of pipe; close duplicated descritor */

         if (pfd[1] != STDOUT_FILENO) {   /* Defensive check */
            if (dup2(pfd[1], STDOUT_FILENO) == -1)
               raiseError("dup2()");
            if (close(pfd[1]) == -1)
               raiseError("close()");
         }

         execlp("ls", "ls", (char *) NULL);     /* Write to pipe */
         raiseError("execlp()");

      default:       /* Parent falls through to create next child */
         break;
   }   

   switch (fork()) {

      case -1:
         raiseError("fork()");

      case 0:        /* Second child: exec 'wc' to read from pipe */
         if (close(pfd[1]) == -1)     /* Write end is unused */
            raiseError("close()");

         /* Duplicate stdin on read end of pipe; close duplicated descriptor */

         if (pfd[0] != STDIN_FILENO) {
            if (dup2(pfd[0], STDIN_FILENO) == -1)
               raiseError("dup2()");
            if (close(pfd[0]) == -1)
               raiseError("close()");
         }

         execlp("wc", "wc", "-l", (char *) NULL);  /* Reads from pipe */
         raiseError("execlp()");

      default:    /* Parent falls through */
         break;
   }
   
   /* Parent closes unused file descriptors for pipe, and waits for children */

   if (close(pfd[0]) == -1)
      raiseError("close()");
   if (close(pfd[1]) == -1)
      raiseError("close()");
   if (wait(NULL) == -1)
      raiseError("wait()");
   if (wait(NULL) == -1)
      raiseError("wait()");

   exit(EXIT_SUCCESS);
}