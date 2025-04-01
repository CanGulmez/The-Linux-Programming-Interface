/* Handling realtime signals */

#include "../linux.h"

static volatile int handlerSleepTime;
static volatile int sigCnt = 0;
static volatile int allDone = 0;

/* Handler for signals established using SA_SIGINFO */
static void siginfoHandler(int sig, siginfo_t *si, void *ucontext)
{
   /* SIGINT or SIGTERM can be used to terminate program */
   if (sig == SIGINT || sig == SIGTERM) {
      allDone = 1;
      return;
   }
   sigCnt ++;
   printf("caught signal %d\n", sig);

   printf("\tsi_signo=%d, si_code=%d (%s), ", si->si_signo, si->si_code,
          (si->si_code == SI_USER) ? "SI_USER" : 
          (si->si_code == SI_QUEUE) ? "SI_QUEUE" : "other");
   printf("si_value=%d\n", si->si_value.sival_int);
   printf("\tsi_pid=%ld, si_uid=%ld\n", (long) si->si_pid, (long) si->si_uid);

   sleep(handlerSleepTime);
}

void main(int argc, char *argv[])
{
   struct sigaction sa;
   int sig;
   sigset_t prevMask, blockMask;

   if (argc > 1 && strcmp(argv[1], "--help") == 0)
      usage_error("Wrong command-line usage");

   printf("%s: PID is %ld\n", argv[0], (long) getpid());

   handlerSleepTime = (argc > 2) ? atoi(argv[2]) : 1;

   /* Establish handler for most signals. During execution of the handler
      mask all other signals to prevent handlers recursively interrupting
      each other (which would make th output hard to read). */

   sa.sa_sigaction = siginfoHandler;
   sa.sa_flags = SA_SIGINFO;
   sigfillset(&sa.sa_mask);

   for (sig = 1; sig < NSIG; sig++)
      if (sig != SIGSTOP && sig != SIGQUIT)
         sigaction(sig, &sa, NULL);

   /* Optionally block signals and sleep, allowing signals to be
      sent to us before they are unblocked and handled */

   if (argc > 1) {
      sigfillset(&blockMask);
      sigdelset(&blockMask, SIGINT);
      sigdelset(&blockMask, SIGTERM);

      if (sigprocmask(SIG_SETMASK, &blockMask, &prevMask) == -1)
         syscall_error();

      printf("%s: signals blocked - sleeping %s seconds\n", argv[0], argv[1]);
      sleep(atoi(argv[1]));
      printf("%s: sleep complete\n", argv[0]);

      if (sigprocmask(SIG_SETMASK, &prevMask, NULL) == -1)
         syscall_error();
      }

   while (!allDone)
   pause();
}
