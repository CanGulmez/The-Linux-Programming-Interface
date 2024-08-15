/* Using a pipe to synhronize multiple processes */

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
   int pfd[2];          /* Process synchronization pipe */
   int i, dummy;

   if (argc < 2 || !strcmp(argv[1], "--help"))
      raiseError("wrong command-line arguments");

   setbuf(stdout, NULL);        /* Make stdout unbuffered, since we 
                                   terminate child with _exit() */
   printf("[%s] Parent started\n", currTime("%T"));

   if (pipe(pfd) == -1)
      raiseError("pipe()");

   for (i=1; i<argc; i++) {
      switch (fork()) {
         
         case -1: 
            raiseError("fork()");
         
         case 0:     /* Child */
            if (close(pfd[0]) == -1)      /* Read end is unused */
               raiseError("close()");

            /* Child does some work, and lets parent know it's done */

            sleep(atoi(argv[i]));

            printf("[%s] Child %d (PID=%ld) closing pipe\n",
                   currTime("%T"), i, (long) getpid());
            
            if (close(pfd[1]) == -1)
               raiseError("close()");

            /* Child now carries on to do other things... */

            _exit(EXIT_SUCCESS);

         default:    /* Parent loops to create next child */
            break;
      }
   }

   /* Parent comes here; close write end of pipe so we can see EOF */

   if (close(pfd[1]) == -1)      /* Write end is unused */
      raiseError("close()");

   /* Parent may do other work, then synchronizes with children */

   if (read(pfd[0], &dummy, 1) != 0)
      raiseError("read()");

   printf("[%s] Parent ready to go\n", currTime("%T"));

   /* Parent can now carry on to do other things... */

   exit(EXIT_SUCCESS);
}
