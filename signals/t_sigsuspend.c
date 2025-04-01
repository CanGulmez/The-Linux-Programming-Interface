/* Using 'sigsuspend()' */

#include "../linux.h"

static volatile sig_atomic_t gotSigquit = 0;

/* Print list of signals within a signal set */
void printSigset(FILE *of, const char *prefix, const sigset_t *sigset)
{
   int sig, cnt;

   cnt = 0;
   for (sig = 1; sig < NSIG; sig ++) {
      if (sigismember(sigset, sig)) {
         cnt ++;
         fprintf(of, "%s%d (%s)\n", prefix, sig, strsignal(sig));
      }
   }
   if (cnt == 0)
      fprintf(of, "%s<empty signal set>\n", prefix);
}

/* Print mask of blocked signals for this process */
int printSigMask(FILE *of, const char *msg)
{
   sigset_t currMask;

   if (msg != NULL)
      fprintf(of, "%s", msg);

   if (sigprocmask(SIG_BLOCK, NULL, &currMask) == -1)
      return -1;

   printSigset(of, "\t\t", &currMask);

   return 0;
}

/* Print signals currently pending for this process */
int printPendingSigs(FILE *of, const char *msg)
{
   sigset_t pendingSigs;

   if (msg != NULL)
      fprintf(of, "%s", msg);

   if (sigpending(&pendingSigs) == -1)
      return -1;

   printSigset(of, "\t\t", &pendingSigs);

   return 0;
}

void handler(int sig)
{
   printf("Caught signal %d (%s)\n", sig, strsignal(sig));
   if (sig == SIGQUIT)
      gotSigquit ++;
}

void main(int argc, char *argv[])
{
   int loopNum;
   time_t startTime;
   sigset_t origMask, blockMask;
   struct sigaction sa;

   printSigMask(stdout, "Initial signal mask is:\n");

   sigemptyset(&blockMask);
   sigaddset(&blockMask, SIGINT);
   sigaddset(&blockMask, SIGQUIT);
   if (sigprocmask(SIG_BLOCK, &blockMask, &origMask) == -1)
      syscall_error();

   sigemptyset(&sa.sa_mask);
   sa.sa_flags = 0;
   sa.sa_handler = handler;

   if (sigaction(SIGINT, &sa, NULL) == -1)
      syscall_error();
   if (sigaction(SIGQUIT, &sa, NULL) == -1)
      syscall_error();

   for (loopNum = 1; !gotSigquit; loopNum++) {
      printf("=== LOOP %d\n", loopNum);

      /* Simulate a critical section by delaying a few seconds */

      printSigMask(stdout, "Starting critical section, signal mask is:\n");
      for (startTime = time(NULL); time(NULL) < startTime + 4; )
         continue;         /* Run for a few seconds elapsed time */

      printPendingSigs(stdout, "Before sigsuspend() - pending signals:\n");

      if (sigsuspend(&origMask) == -1 && errno != EINTR)
         syscall_error();
   }

   if (sigprocmask(SIG_SETMASK, &origMask, NULL) == -1)
      syscall_error();

   printSigMask(stdout, "=== Exited loop\nRestored signal mask to:\n");

   /* Do other processing... */

   exit(EXIT_SUCCESS);
}
