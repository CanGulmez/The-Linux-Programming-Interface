/* Using a pipe to communicate between a parent and child process */

#include "../linux.h"

#define BUF_SIZE     10

void main(int argc, char *argv[])
{
   int pfd[2];             /* Pipe file descriptors */
   char buf[BUF_SIZE];
   ssize_t numRead;

   if (argc != 2 || strcmp(argv[1], "--help") == 0)
      syscall_error();

   if (pipe(pfd) == -1)    /* Create the pipe */
      syscall_error();

   switch (fork()) {
      case -1:
         syscall_error();

      case 0:              /* Child - reads from pipee */
         if (close(pfd[1]) == -1)   /* Write end is unused */
            syscall_error();

         for (;;) {        /* Read data from pipe, echo on stdout */
            numRead = read(pfd[0], buf, BUF_SIZE);
            if (numRead == -1)
               syscall_error();
            if (numRead == 0)
               break;     /* End-of-file */
            if (write(STDOUT_FILENO, buf, numRead) != numRead)
               syscall_error();
         }
         write(STDOUT_FILENO, "\n", 1);
         if (close(pfd[0]) == -1)
            syscall_error();

         _exit(EXIT_SUCCESS);
      
      default:             /* Parent - writes to pipe */
         if (close(pfd[0]) == -1)   /* Read end is unused */
            syscall_error();

         if (write(pfd[1], argv[1], strlen(argv[1])) != strlen(argv[1]))
            syscall_error();

         if (close(pfd[1]) == -1)   /* Child will see EOF */
            syscall_error();

         wait(NULL);       /* Wait for child to finish */   
         exit(EXIT_SUCCESS);   
   }  
}

