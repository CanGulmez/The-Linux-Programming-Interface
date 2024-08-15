/* Using a pipe to communicate between a parent and child processes */

#include "./utils.h"

#define BUF_SIZE     10

void main(int argc, char *argv[])
{
   int pfd[2];       /* Pipe file descriptors */
   char buffer[BUF_SIZE];
   ssize_t numRead;

   if (argc != 2 || !strcmp(argv[1], "--help"))
      raiseError("wrong command line arguments");

   if (pipe(pfd) == -1)       /* Create the pipe */
      raiseError("pipe()");

   switch (fork())
   {
      case -1:
         raiseError("fork()");

      case 0:                 /* Child - reads from pipe */
         if (close(pfd[1]) == -1)      /* Write end is unused */
            raiseError("close()");

         for (;;)             /* Read data from pipe, echo on stdout */
         {
            numRead = read(pfd[0], buffer, BUF_SIZE);
            if (numRead == -1)
               raiseError("read()");
            if (numRead == 0)
               break;
            if (write(STDOUT_FILENO, buffer, numRead) != numRead)
               raiseError("write()");
         }  
         write(STDOUT_FILENO, "\n", 1);
         if (close(pfd[0]) == -1)
            raiseError("close()");
         _exit(EXIT_SUCCESS);

      default:                /* Parent - writes to pipe */
         if (close(pfd[0]) == -1)     /* Read end is unused */
            raiseError("close()");

         if (write(pfd[1], argv[1], strlen(argv[1])) != strlen(argv[1]))
            raiseError("write()");

         if (close(pfd[1]) == -1)      /* Child will see EOF */
            raiseError("close()");
            
         wait(NULL);

         exit(EXIT_SUCCESS);       
   } 
}
