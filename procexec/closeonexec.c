/* Setting the close-on-exec flag for a file descriptor */

#include "../linux.h"

void main(int argc, char *argv[])
{
   int flags;

   if (argc > 1) {
      flags = fcntl(STDOUT_FILENO, F_GETFD);    /* Fetch flags */
      if (flags == -1)
         syscall_error();

      flags |= FD_CLOEXEC;    /* Turn on FD_CLOEXEC */

      if (fcntl(STDOUT_FILENO, F_SETFD, flags) == -1)
         syscall_error();
   }

   execlp("ls", "ls", "-l", argv[0], (char *) NULL);

   syscall_error();
}