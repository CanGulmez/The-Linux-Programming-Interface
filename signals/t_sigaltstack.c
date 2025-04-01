/* Using 'sigaltstack()' */

#include "../linux.h"

static void sigsegvHandler(int sig)
{
   int x;

   printf("Caught signal %d (%s)\n", sig, strsignal(sig));
   printf("Top of handler stack near   %10p\n", (void *) &x);
   fflush(NULL);

   _exit(EXIT_FAILURE);     /* Cannot return after SIGSEGV */
}

/* A recursive function that overflows the stack */
static void overflowStack(int callNum)
{
   char a[100000];            /* Make this stack frame large */

   printf("Call %4d - top of stack near %10p\n", callNum, &a[0]);
   overflowStack(callNum+1);
}

void main(int argc, char *argv[])
{
   stack_t sigstack;
   struct sigaction sa;
   int i;

   printf("Top of standard stack is near %10p\n", (void *) &i);

   /* Allocate alternate stack and inform kernel of its existence */

   sigstack.ss_sp = malloc(SIGSTKSZ);
   if (sigstack.ss_sp == NULL)
      syscall_error();
   sigstack.ss_size = SIGSTKSZ;
   sigstack.ss_flags = 0;
   if (sigaltstack(&sigstack, NULL) == -1)
      syscall_error();
   printf("Alternate stack is at         %10p-%p\n",
          sigstack.ss_sp, (char *) sigstack.ss_sp + sigstack.ss_size);

   sa.sa_handler = sigsegvHandler; /* Establish handler for SIGSEGV */
   sigemptyset(&sa.sa_mask);
   sa.sa_flags = SA_ONSTACK;       /* Handler uses alternate stack */
   if (sigaction(SIGSEGV, &sa, NULL) == -1)
      syscall_error();

   overflowStack(1); 
}
