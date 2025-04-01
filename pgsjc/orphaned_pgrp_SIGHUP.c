/* SIGHUP and orpaned process groups */

#include "../linux.h"

static void handler(int sig)
{
   printf("PID=%ld: caught signal %d (%s)\n", 
      (long) getpid(),  sig, strsignal(sig));   
}

void main(int argc, char *argv[])
{
   int i;
   struct sigaction sa;

   if (argc < 2 || strcmp(argv[1], "--help") == 0)
      syscall_error();

   setbuf(stdout, NULL);      /* Make stdout unbuffered */

   sigemptyset(&sa.sa_mask);
   sa.sa_flags = 0;
   sa.sa_handler = handler;
   if (sigaction(SIGHUP, &sa, NULL) == -1)
      syscall_error();
   if (sigaction(SIGCONT, &sa, NULL) == -1)
      syscall_error();

   printf("parent: PID=%ld, PPID=%ld, PGID=%ld, SID=%ld\n", 
      (long) getpid(), (long) getpid(), (long) getpgrp(), (long) getsid(0));

   /* Create one child for each command-line argument */

   for (i = 1; i < argc; i++) {
      switch(fork()) {
         case -1:
            syscall_error();
         case 0:
            printf("child: PID=%ld, PPID=%ld, PGID=%ld, SID=%ld\n",
               (long) getpid(), (long) getppid(), (long) getpgrp(), (long) getsid(0));

            if (argv[i][0] == 's') {   /* Stop via signal */
               printf("PID=%ld stopping\n", (long) getpid());
               raise(SIGSTOP);
            } else {                   /* Wait for signal */
               alarm(60);
               printf("PID=%ld pausing\n", (long) getpid());
               pause();
            }
            _exit(EXIT_SUCCESS);
         default:
            break;                     /* Parent carries on round loop */
         }
   }

   /* Parent falls through to here after creating all children */

   sleep(3);            /* Give children a change to start */
   printf("parent exiting\n");
   exit(EXIT_SUCCESS);  /* And orphan them and their group */
}
