/* IPv6 case-conversion server using datagram sockets */

#include "./utils.h"

#define BUF_SIZE     10    /* Maximum size of messages exchanged 
                              between client and server */
#define PORT_NUM     50002 /* Server port number */

void main(int argc, char *argv[])
{
   struct sockaddr_in6 svaddr, claddr;
   int sfd, i;
   ssize_t numBytes;
   socklen_t len;
   char buffer[BUF_SIZE];
   char claddrStr[INET6_ADDRSTRLEN];

   sfd = socket(AF_INET6, SOCK_DGRAM, 0);
   if (sfd == -1)
      raiseError("socket()");

   memset(&svaddr, 0, sizeof(struct sockaddr_in6));
   svaddr.sin6_family = AF_INET6;
   svaddr.sin6_addr = in6addr_any;     /* Wildcard address */
   svaddr.sin6_port = htons(PORT_NUM);

   if (bind(sfd, (struct sockaddr *) &svaddr, sizeof(struct sockaddr_in6)) == -1)
      raiseError("bind()");

   /* Receive messages, convert to uppercase, and return to client */

   for (;;)
   {
      len = sizeof(struct sockaddr_in6);
      numBytes = recvfrom(sfd, buffer, BUF_SIZE, 0, (struct sockaddr *) &claddr, &len);

      if (numBytes == -1)
         raiseError("recvfrom");

      if (inet_ntop(AF_INET6, &claddr.sin6_addr, claddrStr, INET6_ADDRSTRLEN) == NULL)
         printf("Couldn't convert client  address to string\n");
      else
         printf("Server received %ld bytes from (%s, %u)\n", 
                (long) numBytes, claddrStr, ntohs(claddr.sin6_port));

      for (i=0; i<numBytes; i++)
         buffer[i] = toupper((unsigned char) buffer[i]);

      if (sendto(sfd, buffer, numBytes, 0, (struct sockaddr *) &claddr, len) != numBytes)
         raiseError("sendto()");
   }
}

