/* Receive message notification via a signal */

#include "../linux.h"

#define NOTIFY_SIG         SIGUSR1

static void handler(int sig)
{
   /* Just interrupt sigsuspend() */
}

void main(int argc, char *argv[])
{
   struct sigevent sev;
   mqd_t mqd;
   struct mq_attr attr;
   void *buffer;
   ssize_t numRead;
   sigset_t blockMask, emptyMask;
   struct sigaction sa;

   if (argc != 2 || strcmp(argv[1], "--help") == 0)
      usage_error("Wrong command-line usage");

   mqd = mq_open(argv[1], O_RDONLY | O_NONBLOCK)
   if (mqd == (mqd_t) -1)
      syscall_error();

   if (mq_getattr(mqd, &attr) == -1)
      syscall_error();

   buffer = malloc(attr.mq_msgsize);
   if (buffer == NULL)
      syscall_error();

   sigemptyset(&blockMask);
   sigaddset(&blockMask, NOTIFY_SIG);
   if (sigprocmask(SIG_BLOCK, &blockMask, NULL) == -1)
      syscall_error();

   sigemptyset(&sa.sa_mask);
   sa.sa_flags = 0;
   sa.sa_handler = handler;
   if (sigaction(NOTIFY_SIG, &sa, NULL) == -1)
      syscall_error();

   sev.sigev_notify = SIGEV_SIGNAL;
   sev.sigev_signo = NOTIFY_SIG;
   if (mq_notify(mqd, &sev) == -1)
      syscall_error();

   sigemptyset(&emptyMask);

   for (;;) {                    /* Wait for notification signal */
      sigsuspend(&emptyMask);

      if (mq_notify(mqd, &sev) == -1)
         syscall_error();

      while ((numRead = mw_recieve(mqd, buffer, attr.mq_msgsize, NULL)) >= 0)
         printf("Read %ld bytes\n", (long) numRead);

      if (errno != EAGAIN)       /* Unexpected error */
         syscall_error();
   }
}
