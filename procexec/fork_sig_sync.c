/* Using signals to synchronize process actions */

#include "../linux.h"

#define SYNC_SIG     SIGUSR1  /* Syncronization signal */
#define BUF_SIZE     1000

static void handler(int sig)  /* Signal handler - does nothing but return */
{

}

char *currTime(const char *format)
{
   static char buffer[BUF_SIZE];
   time_t t;
   size_t s;
   struct tm *tm;

   t = time(NULL);
   tm = localtime(&t);
   if (tm == NULL)
      return NULL;

   s = strftime(buffer, BUF_SIZE, (format != NULL) ? format : "%c", tm);
   
   return (s == 0) ? NULL : buffer;
}


void main(int argc, char *argv[])
{
   pid_t childPid;
   sigset_t blockMask, origMask, emptyMask;
   struct sigaction sa;

   setbuf(stdout, NULL);      /* Disable buffering of stdout */

   sigemptyset(&blockMask);
   sigaddset(&blockMask, SYNC_SIG); /* Block signal */
   if (sigprocmask(SIG_BLOCK, &blockMask, &origMask) == -1)
      syscall_error();

   sigemptyset(&sa.sa_mask);
   sa.sa_flags = SA_RESTART;
   sa.sa_handler = handler;
   if (sigaction(SYNC_SIG, &sa, NULL) == -1)
      syscall_error();

   switch (childPid = fork()) {

      case -1:
         syscall_error();

      case 0:              /* Child */
         
         /* Child does some required action here... */

         printf("[%s %ld] Child started - doing some work\n",
            currTime("%T"), (long) getpid());
         sleep(3);         /* Simulate time spent doing some work */

         /* And then signals parent that it's done */

         printf("[%s %ld] Child about to signal parent\n",
            currTime("%T"), (long) getpid());
         if (kill(getppid(), SYNC_SIG) == -1)
            syscall_error();

         /* Now child can do other things... */

         _exit(EXIT_SUCCESS);

      default:       /* Parent */
            
         /* Parent may do some work here, and then waits for child to
            complete the required action */

         printf("[%s %ld] Parent about to wait for signal\n",
            currTime("%T"), (long) getpid());

         sigemptyset(&emptyMask);
         if (sigsuspend(&emptyMask) == -1 && errno != EINTR)
            syscall_error();
         printf("[%s %ld] Parent got signal\n", currTime("%T"), (long) getpid());

         /* If required, return signal mask to its original state */

         if (sigprocmask(SIG_SETMASK, &origMask, NULL) == -1)
            syscall_error();

         /* Parent carries on to do other things... */

         exit(EXIT_SUCCESS);
   }
}
