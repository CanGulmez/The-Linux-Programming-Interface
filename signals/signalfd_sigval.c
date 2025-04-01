/* Using 'signalfd()' to read signals */

#include "../linux.h"

void main(int argc, char *argv[])
{
   sigset_t mask;
   int sfd, i;
   struct signalfd_siginfo fdsi;
   ssize_t s;

   if (argc < 2 || strcmp(argv[1], "--help") == 0)
      syscall_error();

   printf("%s: PID = %ld\n", argv[0], (long) getpid());

   sigemptyset(&mask);
   for (i = 1; i < argc; i++)
      sigaddset(&mask, atoi(argv[i]));

   if (sigprocmask(SIG_BLOCK, &mask, NULL) == -1)
      syscall_error();

   sfd = signalfd(-1, &mask, 0);
   if (sfd == -1)
      syscall_error();

   for (;;) {
      s = read(sfd, &fdsi, sizeof(struct signalfd_siginfo));
      if (s != sizeof(struct signalfd_siginfo))
         syscall_error();

      printf("%s: got signal %d", argv[0], fdsi.ssi_pid);
      if (fdsi.ssi_code == SI_QUEUE) {
         printf("; ssi_pid = %d; ", fdsi.ssi_pid);
         printf("ssi_int = %d", fdsi.ssi_int);
      }
      printf("\n");
   }
}
