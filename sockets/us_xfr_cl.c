/* A simple UNIX domain strem socket client */

#include "../linux.h"

#define SV_SOCK_PATH       "/tmp/us_xfr"
#define BUF_SIZE           100

void main(int argc, char *argv[])
{
   struct sockaddr_un addr;
   int sfd;
   ssize_t numRead;
   char buf[BUF_SIZE];

   sfd = socket(AF_UNIX, SOCK_STREAM, 0);
   if (sfd == -1)
      syscall_error();

   /* Construct server address, and make the connection */

   memset(&addr, 0, sizeof(struct sockaddr_un));
   addr.sun_family = AF_UNIX;
   strncpy(addr.sun_path, SV_SOCK_PATH, sizeof(addr.sun_path) - 1);

   if (connect(sfd, (struct sockaddr *) &addr, 
         sizeof(struct sockaddr_un)) == -1)
      syscall_error();

   /* Copy stdin to socket */

   while ((numRead = read(STDIN_FILENO, buf, BUF_SIZE)) > 0)
      if (write(sfd, buf, numRead) != numRead)
         usage_error("Partial/failed write");

   if (numRead == -1)
      syscall_error();

   exit(EXIT_SUCCESS);
}
