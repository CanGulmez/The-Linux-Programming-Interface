/* This program demonstrates that how to get info from a signal. */

#include "./utils.h"

void handler(int sig, siginfo_t *siginfo, void *ucontext)
{
   if (sig == SIGINT)
   {
      printf("\n");
      printf("si_code: %d\n", siginfo->si_code);
      printf("si_errno: %d\n", siginfo->si_errno);
      printf("si_signo: %d\n", siginfo->si_signo);  
   }
   exit(EXIT_SUCCESS);
}

void main(int argc, char *argv[])
{
   struct sigaction sa;

   sigemptyset(&sa.sa_mask);
   sa.sa_flags = SA_SIGINFO;
   sa.sa_sigaction = handler; 

   if (sigaction(SIGINT, &sa, NULL) == -1)
      raiseError("sigaction()");

   /* Loop forever... */ 
   for (int i=0; ; i++)
   {
      printf("%d\n", i);
      sleep(1);
   }

   exit(EXIT_SUCCESS);
}
