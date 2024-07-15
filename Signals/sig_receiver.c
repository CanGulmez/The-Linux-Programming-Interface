/* Catching and counting signals 

   This program is designed to catch and report statistics on signals
   sent by the 'sig_sender' program.
*/

#include "./utils.h"

static int sigCnt[NSIG];          /* Counts deliveries of each signal */
static volatile sig_atomic_t gotSigint = 0;  
                                  /* Set nonzero if SIGINT is delivered */

void handler(int sig)
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
   for (sig=1; sig<NSIG; sig++)
      if (sigismember(sigset, sig))
         cnt++,
         fprintf(of, "%s%d (%s)\n", prefix, sig, strsignal(sig));

   if (cnt == 0)
      fprintf(of, "%s<empty signal set>\n", prefix);
}

void main(int argc, char *argv[])
{
   int n, numSecs;
   sigset_t pendingMask, blockingMask, emptyMask;

   printf("%s: PID is %ld\n", argv[0], (long) getpid());

   for (n=1; n<NSIG; n++)        /* Same handler for all signals */
      (void) signal(n, handler); /* Ignore errors */

   /* If a sleep time was specified, temporarily block all signals,
      sleep (while another process sends us signals), and then
      display the mask of pending signals and unblock all signals */

   if (argc > 1)
   {
   numSecs = atoi(argv[1]);

   sigfillset(&blockingMask);
   if (sigprocmask(SIG_SETMASK, &blockingMask, NULL) == -1)
      raiseError("sigprocmask()");

   printf("%s: sleeping for %d seconds\n", argv[0], numSecs);
   sleep(numSecs);

   if (sigpending(&pendingMask) == -1)
      raiseError("sigpending()");

   printf("%s: pending signals are: \n", argv[0]);
   printSigset(stdout, "\t\t", &pendingMask);

   sigemptyset(&emptyMask);         /* Unblock all signals */
   if (sigprocmask(SIG_SETMASK, &emptyMask, NULL) == -1)
      raiseError("sigprocmask()");
   }

   while (!gotSigint)            /* Loop until SIGINT caught */
      continue;

   for (n=1; n<NSIG; n++)        /* Display number of signals received */
      if (sigCnt[n] != 0)
         printf("%s: signal %d caught %d times%s\n", argv[0], n, 
                sigCnt[n], (sigCnt[n] == 1) ? "" : "s");

   exit(EXIT_SUCCESS);
}
