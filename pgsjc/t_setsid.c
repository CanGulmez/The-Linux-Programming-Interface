/* Creating a new session */

#include "../linux.h"

void main(int argc, char *argv[])
{
   if (fork() != 0)           /* Exit if parent, or on error */
      _exit(EXIT_SUCCESS);

   if (setsid() == -1)
      syscall_error();

   printf("PID=%ld, PGID=%ld, SID=%ld\n", (long) getpid(),
      (long) getpgrp(), (long) getsid(0));

   if (open("/dev/tty", O_RDWR) == -1)
      syscall_error();

   exit(EXIT_SUCCESS);
}
