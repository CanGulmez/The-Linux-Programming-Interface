/* A simple UNIX domain datagram server */

#include "./utils.h"

#define SV_SOCK_PATH "/tmp/ud_ucase"
#define BUF_SIZE     10    /* Maximum size of messages exchanged 
                              between client to server */

void main(int argc, char *argv[])
{
   struct sockaddr_un svaddr, claddr;
   int sfd, i;
   ssize_t numBytes;
   socklen_t len;
   char buffer[BUF_SIZE];

   sfd = socket(AF_UNIX, SOCK_DGRAM, 0); /* Create server socket */
   if (sfd == -1)
      raiseError("socket()");

   /* Construct well-known address and bind server socket to it */

   if (remove(SV_SOCK_PATH) == -1 && errno != ENOENT)
      raiseError("remove()");

   memset(&svaddr, 0, sizeof (struct sockaddr_un));
   svaddr.sun_family = AF_UNIX;
   strncpy(svaddr.sun_path, SV_SOCK_PATH, sizeof(svaddr.sun_path) - 1);

   if (bind(sfd, (struct sockaddr *) &svaddr, sizeof (struct sockaddr_un)) == -1)
      raiseError("bind()");

   /* Receive messages, convert to uppercase, and return to client */

   for (;;)
   {
      len = sizeof (struct sockaddr_un);
      numBytes = recvfrom(sfd, buffer, BUF_SIZE, 0, (struct sockaddr *) &claddr, &len);

      if (numBytes == -1)
         raiseError("recvfrom()");

      printf("Server received %ld bytes from %s\n", (long) numBytes, claddr.sun_path);

      for (i=0; i<numBytes; i++)
         buffer[i] = toupper((unsigned char) buffer[i]);

      if (sendto(sfd, buffer, numBytes, 0, (struct sockaddr *) &claddr, len) != numBytes)
         raiseError("sendto()");
   }
}
