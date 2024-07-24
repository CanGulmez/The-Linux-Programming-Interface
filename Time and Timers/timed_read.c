/* Performing a 'read()' with timeout 

   This program demonstrates this technique for 'read()', using 'alarm()'
   to establish the timer.
*/

#include "./utils.h"

/* SIGALRM handler: interrupts blocked system call */
void handler(int sig)
{
   printf("Caught signal\n");    /* UNSAFE (see Section 21.1.2) */
}

void main(int argc, char *argv[])
{
   struct sigaction sa;
   char buffer[BUFSIZ];
   ssize_t numRead;
   int savedErrno;

   if (argc > 1 && !strcmp(argv[1], "--help"))
      raiseError("wrong command-line arguments");

   /* Set up handler for SIGALRM. Allow system calls to be interrupted,
      unless second command-line argument was supplied. */

   
sa.sa_flags = (argc > 2) ? SA_RESTART : 0;
   sigemptyset(&sa.sa_mask);
   sa.sa_handler = handler;
   if (sigaction(SIGALRM, &sa, NULL) == -1)
      raiseError("sigaction()");
   alarm((argc > 1) ? atoi(argv[1]) : 10);

   numRead = read(STDIN_FILENO, buffer, BUFSIZ - 1);

   savedErrno = errno;                 /* In case alarm() changes it */
   alarm(0);                           /* Ensure timer is turned off */
   errno = savedErrno;

   /* Determine result of read() */

   if (numRead == -1)
   {
      if (errno == EINTR)
         printf("Read timed out\n");
      else
         raiseError("read()");
   }
   else 
      printf("Successful read (%ld bytes): %.*s", 
             (long) numRead, (int) numRead, buffer);

   exit(EXIT_SUCCESS);
}
 