/* Calling a nonentrant fucntion from both 'main()' and signal handler 

   This program demonstrates the nonreentrant nature of `crypt()` function.
   As command-line arguments, this program accepts two strings.
*/

#include "./utils.h"

static char *str2;         /* Set from argv[2] */
static int handled = 0;    /* Counts number of calls to handler */

void handler(int sig)
{
   crypt(str2, "xx"); 
   handled ++;
}

char *currTime(const char *format)
{
   static char buffer[BUFSIZ];      /* Noreentrant */
   time_t t;
   size_t s;
   struct tm *tm;

   t = time(NULL);
   tm = localtime(&t);
   if (tm == NULL)   
      return NULL;

   s = strftime(buffer, BUFSIZ, (format != NULL) ? format : "%c", tm);

   return (s == 0) ? NULL : buffer;
}

void main(int argc, char *argv[])
{
   char *cr1;
   int callNum, mismatch;
   struct sigaction sa;

   if (argc != 3)
      raiseError("wrong command-line arguments");

   str2 = argv[2];  /* Make argv[2] avaliable to handler */
   cr1 = strdup(crypt(argv[1], "xx")); /* Copy statically allocated string
                                          to another buffer */

   if (cr1 == NULL)
      raiseError("strdup()");

   sigemptyset(&sa.sa_mask);
   sa.sa_flags = 0; 
   sa.sa_handler = handler;

   if (sigaction(SIGINT, &sa, NULL) == -1)
      raiseError("sigaction()");

   /* Repeatedly call crypt() using argv[1]. If interrupted by a
      signal handler, then the static storage returned by crypt()
      will be overwritten by the results of encrypting argv[2], and
      strcmp() will detect a mismatch with the value in 'cr1'. */

   for (callNum = 1, mismatch = 0; ;callNum++)
   {
      if (strcmp(crypt(argv[1], "xx"), cr1) != 0)
      {
         mismatch ++;
         printf("[%s] Mismatch on call %d (mismatch=%d handled=%d)\n", 
                currTime("%F %T"), callNum, mismatch, handled);
      }
   }
   exit(EXIT_SUCCESS);
}
