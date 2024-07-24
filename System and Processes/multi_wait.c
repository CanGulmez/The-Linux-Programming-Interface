/* Creating and waiting for multiple children 

   The program demonstrates the use of 'wait()'. This program creates 
   multiple child processes, one per (integer) command-line argument. Each 
   child sleeps for the number of seconds specified in the corresponding
   command-line argument and then exits
*/

#include "./utils.h"

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
   int numDead;               /* Number of children so far waited for */
   pid_t childPid;            /* PID of waited for child */
   int i;

   if (argc < 2 || !strcmp(argv[1], "--help"))
      raiseError("wrong command-line arguments");

   setbuf(stdout, NULL);         /* Disable buffering of stdout */

   for (i=1; i<argc; i++)        /* Create one child for each argument */
   {
      switch (fork()) 
      {
         case -1:
            raiseError("fork()");
         
         case 0:                 /* Child sleeps for a while then exits */
            printf("[%s] child %d started with PID %ld, sleeping %s "
                   "seconds\n", currTime("%F %T"), i, (long) getpid(), argv[i]);
            sleep(atoi(argv[i]));
            _exit(EXIT_SUCCESS);
         
         default:                /* Parent just continues around loop */
            break;
      }
   }

   numDead = 0;
   for (;;)                      /* Parent waits for each child to exit */
   {
      childPid = wait(NULL);
      if (childPid == -1)
      { 
         if (errno == ECHILD)
         {
            printf("No more children - bye!\n");
            exit(EXIT_SUCCESS);
         }
         else                     /* Some other (unexpected) error */
            raiseError("wait()");
      }
      numDead ++;
      printf("[%s] wait() returned child PID %ld (numDead=%d)\n",
             currTime("%F %T"), (long) childPid, numDead);
   }
}
