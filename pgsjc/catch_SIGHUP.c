/* Catching SIGHUP */

#include "../linux.h"

static void handler(int sig)
{

}
 
void main(int argc, char *argv[])
{
   pid_t childPid;
   struct sigaction sa;

   setbuf(stdout, NULL);         /* Make stdout unbuffered */

   sigemptyset(&sa.sa_mask);
   sa.sa_flags = 0;
   sa.sa_handler = handler;
   if (sigaction(SIGHUP, &sa, NULL) == -1)
      syscall_error();

   childPid = fork();
   if (childPid == -1)
      syscall_error();

   if (childPid == 0 && argc > 1)
      if (setpgid(0, 0) == -1)      /* Move to new proces group */
         syscall_error();

   printf("PID=%ld; PPID=%ld; PGID=%ld; SID=%ld\n", (long) getpid(),
      (long) getppid(), (long) getpgrp(), (long) getsid(0));

   alarm(60);     /* An unhandled SIGALRM ensures this process
                     will die if nothing else terminates it */
   for (;;) {
      pause();
      printf("%ld: caught SIGHUP\n", (long) getpid());
   }
}
