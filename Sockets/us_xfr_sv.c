/* A simple UNIX domain stream socket server */

#include "./utils.h"

#define SV_SOCK_PATH    "/tmp/us_cfr"
#define BUF_SIZE        100
#define BACKLOG         5

void main(int argc, char *argv[])
{
   struct sockaddr_un addr;
   int sfd, cfd;
   ssize_t numRead;
   char buffer[BUF_SIZE];

   sfd = socket(AF_UNIX, SOCK_STREAM, 0);
   if (sfd == -1)
      raiseError("socket()");

   /* Construct server socket address, bind socket to it, 
      and make this a listening socket */

   if (remove(SV_SOCK_PATH) == -1 && errno != ENOENT)
      raiseError("remove()");

   memset(&addr, 0, sizeof(struct sockaddr_un));
   addr.sun_family = AF_UNIX;
   strncpy(addr.sun_path, SV_SOCK_PATH, sizeof(addr.sun_path) - 1);

   if (bind(sfd, (struct sockaddr *) &addr, sizeof(struct sockaddr_un)) == -1)
      raiseError("bind()");

   if (listen(sfd, BACKLOG) == -1)
      raiseError("listen()");

   for (;;)             /* Handle client connections iteratively */
   {
      /* Accept a connection. The connection is returned on a new 
         socket, 'cfd'; the listening socket ('sfd') remains open 
         and can be used to accept further connections. */

      cfd = accept(sfd, NULL, NULL); 
      if (cfd == -1)
         raiseError("accept()");

      /* Transfer data from connected to stdout until EOF */

      while ((numRead = read(cfd, buffer, BUF_SIZE)) > 0)
         if (write(STDOUT_FILENO, buffer, numRead) != numRead)
            raiseError("write()");
      
      if (numRead == -1)
         raiseError("read()");

      if (close(cfd) == -1)
         raiseError("close()");
   }
}
