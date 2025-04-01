/* A simple UNIX domain datagram server */

#include "../linux.h"

#define BUF_SIZE         10
#define SV_SOCK_PATH    "/tmp/ud_ucase"

void main(int argc, char *argv[])
{
   struct sockaddr_un svaddr, claddr;
   int sfd, i;
   ssize_t numBytes;
   socklen_t len;
   char buf[BUF_SIZE];

   sfd = socket(AF_UNIX, SOCK_DGRAM, 0);
   if (sfd == -1)
      syscall_error();

   /* Construct well-known address and bind server socket to it */

   memset(&svaddr, 0, sizeof(struct sockaddr_un));
   svaddr.sun_family = AF_UNIX;
   strncpy(svaddr.sun_path, SV_SOCK_PATH, sizeof(svaddr.sun_path) - 1);

   if (bind(sfd, (struct sockaddr *) &svaddr, sizeof(struct sockaddr_un)) == -1)
      syscall_error();

   /* Receive messages, convert to uppercase, and return to client */

   for (;;) {
      len = sizeof(struct sockaddr_un);
      numBytes = recvfrom(sfd, buf, BUF_SIZE, 0, 
                         (struct sockaddr *) &claddr, &len);
      if (numBytes == -1)
         syscall_error();

      printf("Server received %ld bytes from %s\n", (long) numBytes,
         claddr.sun_path);

      for (i = 0; i < numBytes; i++)
         buf[i] = toupper((unsigned char) buf[i]);

      if (sendto(sfd, buf, numBytes, 0, (struct sockaddr *) &claddr, 
            len) != numBytes)
         usage_error("Parital sento");
   }
}