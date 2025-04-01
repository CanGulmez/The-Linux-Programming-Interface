/* Performing a 'read()' with timeout */

#include "../linux.h"

#define BUF_SIZE     200

static void handler(int sig) 
{
   printf("Caught signal\n");
}

void main(int argc, char *argv[])
{
   struct sigaction sa;
   char buf[BUF_SIZE];
   ssize_t numRead;
   int savedErrno;
   
   if (argc > 1 && strcmp(argv[1], "--help") == 0)
      usage_error("Wrong command-line usage");  

   /* Set up handler for SIGALRM. Allow system calls to 
      be interrupted, unless second command-line argument 
      was supplied. */

   sa.sa_flags = (argc > 2) ? SA_RESTART : 0;
   sigemptyset(&sa.sa_mask);
   sa.sa_handler = handler;
   if (sigaction(SIGALRM, &sa, NULL) == -1)
      syscall_error();

   alarm((argc > 1) ? atoi(argv[1]) : 10);

   numRead = read(STDIN_FILENO, buf, BUF_SIZE);

   savedErrno = errno;     /* In case alarm() changes it */
   alarm(0);               /* Ensure alarm is turned off */
   errno = savedErrno;

   /* Determine result of read() */
   
   if (numRead == -1) {
      if (errno == EINTR)
         printf("Read timed out\n");
      else
         syscall_error();
   } else {
      printf("Successful read (%ld bytes): %.*s", 
         (long) numRead, (int) numRead, buf);
   }
   exit(EXIT_SUCCESS);
}
