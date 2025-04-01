/* Catching and counting signals */

#include "../linux.h"

static int sigCnt[NSIG];
static volatile sig_atomic_t gotSigint = 0;

static void handler(int sig)
{
   if (sig == SIGINT)
      gotSigint = 1;
   else
      sigCnt[sig]++;
}

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

void main(int argc, char *argv[])
{
   int n, numSecs;
   sigset_t pendingMask, blockingMask, emptyMask;

   printf("%s: PID is %ld\n", argv[0], (long) getpid());

   for (n = 1; n < NSIG; n++)    /* Same handler for all signals */
      (void) signal(n, handler); /* Ignore errors */

   /* If a sleep time was specified, temporarily block all signals,
      sleep (while another process sends us signals), and then
      display the mask of pending signals and unblock all signals */

   if (argc > 1)
      numSecs = atoi(argv[1]);

   sigfillset(&blockingMask);
   if (sigprocmask(SIG_SETMASK, &blockingMask, NULL) == -1)
      syscall_error();

   printf("%s: sleeping for %d seconds\n", argv[0], numSecs);
   sleep(numSecs);

   if (sigpending(&pendingMask) == -1)
      syscall_error();

   printf("%s: pending signals are: \n", argv[0]);
   printSigset(stdout, "\t\t", &pendingMask);

   sigemptyset(&emptyMask);      /* Unblock all signals */
   if (sigprocmask(SIG_SETMASK, &emptyMask, NULL) == -1)
      syscall_error();

   while (!gotSigint)
      continue;

   for (n = 1; n < NSIG; n++)
      if (sigCnt[n] != 0)
         printf("%s: signal %d caughtt %d time%s\n", argv[0], n, 
            sigCnt[n], (sigCnt[n] == 1) ? "" : "s");

   exit(EXIT_SUCCESS);

   struct sigaction action;

   
}
