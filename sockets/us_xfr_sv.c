/* A simple UNIX domain stream socket server */

#include "../linux.h"

#define SV_SOCK_PATH       "/tmp/us_xfr"
#define BUF_SIZE           100
#define BACKLOG            5

void main(int argc, char *argv[])
{
   struct sockaddr_un addr;
   int sfd, cfd;
   ssize_t numRead;
   char buf[BUF_SIZE];

   sfd = socket(AF_UNIX, SOCK_STREAM, 0);
   if (sfd == -1)
      syscall_error();

   memset(&addr, 0, sizeof(struct sockaddr_un));
   addr.sun_family = AF_UNIX;
   strncpy(addr.sun_path, SV_SOCK_PATH, sizeof(addr.sun_path) -1);

   if (bind(sfd, (struct sockaddr *) &addr, sizeof(struct sockaddr_un)) == -1)
      syscall_error();

   if (listen(sfd, BACKLOG) == -1)
      syscall_error();

   for (;;) {     /* Handle client connections iteratively */

      /* Accept a connection. The connection is returned on a nwq
         socket, 'cfd'; the listening socket ('sfd') remains open
         and can be used to accept furter connections. */

      cfd = accept(sfd, NULL, NULL);
      if (cfd == -1)
         syscall_error();

      /* Transfer data from connected socket to stdout until EOF */

      while ((numRead = read(cfd, buf, BUF_SIZE)) > 0)
         if (write(STDOUT_FILENO, buf, BUF_SIZE) != numRead)
            usage_error("Partial/failed write");

      if (numRead == -1)
         syscall_error();

      if (close(cfd) == -1)
         syscall_error();
   }
}