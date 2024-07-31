/* Handling realtime signals 

   The program provides an example of handling realtime signals. This program
   catches signals and displays various fields from the `siginfo_t` strcuture
   passed to the signal handler. The program takes two optional integer 
   command-line arguments. If the first argument is supplied, the main program
   blocks all signals, and then sleeps for the number of seconds specified
   by this argument. During this time, we can queue multiple realtime signals
   to the process and observe what hapens when the signal are unblocked. The
   second argument specifies the number of seconds that signal handler should
   sleep before returning.
*/

#include "./utils.h"

static volatile int handlerSleepTime;
static volatile int sigCnt = 0;           /* Number of signals received */
static volatile int allDone = 0;

/* Handler for signals established using SA_SIGINFO */
void siginfoHandler(int sig, siginfo_t *si, void *ucontext)
{
   /* UNSAFE: This handler uses non-async-signal-safe functions
      (printf()); see Section 21.1.2) */

   /* SIGINT and SIGTERM can be used to terminate program */

   if (sig == SIGINT || sig == SIGTERM)
   {
      allDone = 1;
      return;
   }

   sigCnt ++;
   printf("caught signal %d\n", sig); 

   printf("    si_signo=%d, si_code=%d (%s), ", si->si_signo, si->si_code, 
          (si->si_code == SI_USER) ? "SI_USER" : 
          (si->si_code == SI_QUEUE) ? "SI_QUEUE" : "other");
   printf("si_value=%d\n", si->_sifields._rt.si_sigval.sival_int);
   // printf("    si_pid=%ld, si_uid=%ld\n", (long) si->_sifields._kill.si_pid,
   //        (long) si->_sifields._kill.si_uid);

   sleep(handlerSleepTime);
}  

void main(int argc, char *argv[])
{
   struct sigaction sa;
   int sig;
   sigset_t prevMask, blockMask;

   if (argc > 1 && !strcmp(argv[1], "--help"))
      raiseError("wrong command-line argument");

   printf("%s: PID is %ld\n", argv[0], (long) getpid());

   handlerSleepTime = (argc > 2) ? atoi(argv[2]) : 1;

   /* Establish handler for most signals. During execution of the handler,
      mask all other signals to prevent handlers recursively interrupting
      each other (which would make the output hard to read). */

   sa.sa_sigaction = siginfoHandler;
   sa.sa_flags = SA_SIGINFO;
   sigfillset(&sa.sa_mask);

   for (sig=1; sig<NSIG; sig++)
      if (sig != SIGTSTP && sig != SIGQUIT)
         sigaction(sig, &sa, NULL);

   /* Optionally block signals and sleep, allowing signals to be
      sent to us before they are unblocked and handled */

   if (argc > 1)
   {
      sigfillset(&blockMask);
      sigaddset(&blockMask, SIGINT);
      sigdelset(&blockMask, SIGTERM);

      if (sigprocmask(SIG_SETMASK, &prevMask, NULL) == -1)
         raiseError("sigprocmask()");

      printf("%s: signals blocked - sleeping %s seconds\n", argv[0], argv[1]);
      sleep(atoi(argv[1]));
      printf("%s: sleep complete\n", argv[0]);

      if (sigprocmask(SIG_SETMASK, &prevMask, NULL) == -1)
         raiseError("sigprocmask()");
   }

   while (!allDone)        /* Wait for incoming signals */
      pause();
}
