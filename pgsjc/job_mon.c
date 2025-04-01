/* Observing the treatment of a process under job control */

#include "../linux.h"

static int cmdNum;         /* Our position in pipeline */

static void handler(int sig)
{
   if (getpid() == getpgrp())    /* If process group leader */
      fprintf(stderr, "Terminal FG process group: %ld\n",
         (long) tcgetpgrp(STDERR_FILENO));
   fprintf(stderr, "Process %ld (%d) received signal %d (%s)\n",
      (long) getpid(), cmdNum, sig, strsignal(sig));

   /* If we catch, it won't actually stop up. Therefore we
      raise SIGSTOP so we acutally get stopped. */

   if (sig == SIGTSTP)
      raise(SIGSTOP);
}

void main(int argc, char *argv[])
{
   struct sigaction sa;

   sigemptyset(&sa.sa_mask);
   sa.sa_flags = SA_RESTART;
   sa.sa_handler = handler;
   if (sigaction(SIGINT, &sa, NULL) == -1)
      syscall_error();
   if (sigaction(SIGTSTP, &sa, NULL) == -1)
      syscall_error();
   if (sigaction(SIGCONT, &sa, NULL) == -1)
      syscall_error();

   /* If stdin is a terminal, this is the first process in pipeline:
      print a heading and initialize message to be sent dowm pipe */

   if (isatty(STDIN_FILENO)) {
      fprintf(stderr, "Terminal FG process group: %ld\n",
         (long) tcgetpgrp(STDIN_FILENO));
      fprintf(stderr, "Command  PID  PPID  PGRP   SID\n");
      cmdNum = 0;
   } else {
      if (read(STDIN_FILENO, &cmdNum, sizeof(cmdNum)) <= 0)
         syscall_error();
   }

   cmdNum ++;
   fprintf(stderr, "%4d    %5ld %5ld %5ld %5ld\n", cmdNum,
      (long) getpid(), (long) getppid(), (long) getpgrp(), (long) getsid(0));

   /* If not the last process, pass a message to the next process */

   if (!isatty(STDOUT_FILENO))   /* If not tty, then should be pipe */
      if (write(STDOUT_FILENO, &cmdNum, sizeof(cmdNum)) == -1)
         syscall_error();

   for (;;)       /* Wait for signals */
      pause();
}