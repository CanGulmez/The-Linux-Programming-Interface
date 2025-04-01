/* Catching SIGHUP when a terminal disconnect occurs */

#include "../linux.h"

/* Handler for SIGHUP */
static void handler(int sig)
{
   printf("PID %ld: caught signal %2d (%s)\n", (long) getpid(),
   sig, strsignal(sig));
}

void main(int argc, char *argv[])
{
   pid_t parentPid, childPid;
   int i;
   struct sigaction sa;

   if (argc < 2 || strcmp(argv[1], "--help") == 0)
      syscall_error();

   setbuf(stdout, NULL);      /* Make stdout unbuffered */

   parentPid = getpid();
   printf("PID of parent process is:       %ld\n", (long) parentPid);
   printf("Foreground process group ID is: %ld\n", 
      (long) tcgetpgrp(STDIN_FILENO));

   for (i = 1; i < argc; i++) {  /* Create child processes */
      childPid = fork();
      if (childPid == -1)
         syscall_error();

      if (childPid == 0) {
         if (argv[i][0] == 'd')
            if (setpgid(0, 0) == -1)
               syscall_error();

         sigemptyset(&sa.sa_mask);
         sa.sa_flags = 0;
         sa.sa_handler = handler;
         if (sigaction(SIGHUP, &sa, NULL) == -1)
            syscall_error();
         
         break;                  /* Child exits loop */
      }
   }

   /* All processes fall through to here */

   alarm(60);      /* Ensure each process evetually terminates */

   printf("PID=%ld PGID=%ld\n", (long) getpid(), (long) getpgrp());
   for (;;)
      pause();    /* Wait for signals */
}