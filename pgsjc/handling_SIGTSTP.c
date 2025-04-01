/* Handling SIGTSTP */

#include "../linux.h"

static void tstpHandler(int sig)
{
   sigset_t tstpMask, prevMask;
   int savedErrno;
   struct sigaction sa;

   savedErrno = errno;        /* In case we change 'errno' here */

   printf("Caught SIGTSTP\n");

   if (signal(SIGTSTP, SIG_DFL) == SIG_ERR)
      syscall_error();        /* Set handling to default */

   raise(SIGTSTP);            /* Generate a further SIGTSTP */

   /* Unblock SIGTSTP; then pending SIGTSTP immediately suspends the program */

   sigemptyset(&tstpMask);
   sigaddset(&tstpMask, SIGTSTP);
   if (sigprocmask(SIG_UNBLOCK, &tstpMask, &prevMask) == -1)
      syscall_error();

   /* Execution resumes here after SIGCONT */

   if (sigprocmask(SIG_SETMASK, &prevMask, NULL) == -1)
      syscall_error();        /* Reblock SIGTSTP */

   sigemptyset(&sa.sa_mask);  /* Reestablish handler */
   sa.sa_flags = SA_RESTART;
   sa.sa_handler = tstpHandler;
   if (sigaction(SIGTSTP, &sa, NULL) == -1)
      syscall_error();

   printf("Exiting SIGTSTP handler\n");
   errno = savedErrno;
}

void main(int argc, char *argv[])
{
   struct sigaction sa;

   /* Only establish handler for SIGTSTP if it is not being ignored */

   if (sigaction(SIGTSTP, NULL, &sa) == -1)
      syscall_error();

   if (sa.sa_handler != SIG_IGN) {
      sigemptyset(&sa.sa_mask);
      sa.sa_flags = SA_RESTART;
      sa.sa_handler = tstpHandler;
      if (sigaction(SIGTSTP, &sa, NULL) == -1)
         syscall_error();
   }

   for (;;) {                 /* Wait for signals */
      pause();
      printf("Main\n");
   }
}
