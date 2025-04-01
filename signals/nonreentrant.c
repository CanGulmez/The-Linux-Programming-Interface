/* Calling a nonreentrant function from both 'main()' and a signal handler */

#include "../linux.h"

static char *str2;               /* Set from argv[2] */
static int handled = 0;          /* Counts number of calls to handler */

void handler(int sig)
{
   crypt(str2, "xx");
   handled ++;
}

void main(int argc, char *argv[])
{
   char *cr1;
   int callNum, mismatch;
   struct sigaction sa;

   if (argc != 3)
      usage_error("Wrong command-line usage");
   
   str2 = argv[2];
   cr1 = strdup(crypt(argv[1], "xx"));

   if (cr1 == NULL)
      syscall_error();

   sigemptyset(&sa.sa_mask);
   sa.sa_flags = 0;
   sa.sa_handler = handler;
   if (sigaction(SIGINT, &sa, NULL) == -1)
      syscall_error();

   /* Repeatedly call crypt() using argv[1]. If interrupted by a
      signal handler, then the static storage returned by crypt() 
      will be overwritten by the results of encrypting argv[2], and
      strcmp() will detect a mismatch with the value in 'cr1'. */

   for (callNum = 1, mismatch = 0; ; callNum) {
      if (strcmp(crypt(argv[1], "xx"), cr1) != 0) {
         mismatch ++;
         printf("Mismatch on call %d (mismatch=%d handled=%d)\n",
            callNum, mismatch, handled);
      }
   }
   exit(EXIT_SUCCESS);
}
