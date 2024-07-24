/* Using signals to synchronize process actions 

   In this program, we assume that it is the parent that must wait on the
   child to carry out some action. The signal-related calls in the parent
   and child can be swapped if the child must wait on the parent. It is
   even possible for both parent and child to signal eachh other multiple
   times in order to coordinate their actions, although, in practice, 
   such coordination is more likely to be done using semaphores, file
   locks, or message passing.
*/

#include "./utils.h"

#define SYNC_SIG        SIGUSR1     /* Synchronization signal */

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

void handler(int sig)
{

}

void main(int argc, char *argv[])
{
   pid_t childPid;
   sigset_t blockMask, origMask, emptyMask;
   struct sigaction sa;

   setbuf(stdout, NULL);         /* Disable buffering of stdout */

   sigemptyset(&blockMask);
   sigaddset(&blockMask, SYNC_SIG); /* Block signal */
   if (sigprocmask(SIG_BLOCK, &blockMask, &origMask) == -1)
      raiseError("sigprocmask()");

   sigemptyset(&sa.sa_mask);
   sa.sa_flags = SA_RESTART;
   sa.sa_handler = handler;
   if (sigaction(SYNC_SIG, &sa, NULL) == -1)
      raiseError("sigaction()");

   switch (childPid = fork())
   {
      case -1:
         raiseError("fork()");

      case 0:     /* Child */

         /* Child does some required action here... */

         printf("[%s %ld] Child started -doing some work\n", 
                currTime("%F %T"), (long) getpid());
         sleep(2);               /* Simulate time spent doing some work */

         /* And then signals parent that it's done */

         printf("[%s %ld] Child about to signal parent\n", 
                currTime("%F %T"), (long) getpid());
         
         if (kill(getppid(), SYNC_SIG) == -1)
            raiseError("kill()");

         /* Now child can do other things... */

         _exit(EXIT_SUCCESS);

      default:    /* Parent */

         /* Parent may do some work here, and then waits for child to
            complete the required action */

         printf("[%s %ld] Parent about to wait for signal\n",  
                currTime("%F %T"), (long) getpid());

         sigemptyset(&emptyMask);
         if (sigsuspend(&emptyMask) == -1 && errno != EINTR)
            raiseError("sissuspend()");

         printf("[%s %ld] Parent got signal\n", 
                currTime("%F %T"), (long) getpid());
         
         /* If required, return signal mask to its original state */

         if (sigprocmask(SIG_SETMASK, &origMask, NULL) == -1)
            raiseError("sigprocmask()");

         /* Parent carries on to do other things... */

         exit(EXIT_SUCCESS);
   }
}
