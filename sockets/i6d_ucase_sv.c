/* IPv6 case-conversion server using datagram sockets */

#include "../linux.h"

#define BUF_SIZE     10
#define PORT_NUM     50002

void main(int argc, char *argv[])
{
   struct sockaddr_in6 svaddr, claddr;
   int sfd, i;
   ssize_t numBytes;
   socklen_t len;
   char buf[BUF_SIZE];
   char claddrStr[INET6_ADDRSTRLEN];

   sfd = socket(AF_INET6, SOCK_DGRAM, 0);
   if (sfd == -1)
      syscall_error();

   memset(&svaddr, 0, sizeof(struct sockaddr_in6));
   svaddr.sin6_family = AF_INET6;
   svaddr.sin6_addr = in6addr_any;
   svaddr.sin6_port = htons(PORT_NUM);

   if (bind(sfd, (struct sockaddr *) &svaddr,
         sizeof(struct sockaddr_in6)) == -1)
      syscall_error();

   /* Receive messages, convert to uppercase, and return to client */

   for (;;) {
      len = sizeof(struct sockaddr_in6);
      numBytes = recvfrom(sfd, buf, BUF_SIZE, 0,
         (struct sockaddr *) &claddr, &len);
      if (numBytes == -1)
         syscall_error();

      if (inet_ntop(AF_INET6, &claddr.sin6_addr, claddrStr,
            INET6_ADDRSTRLEN) == NULL)
         printf("Couldn't convert client address to string\n");
      else
         printf("Server received %ld bytes from (%s, %u)\n",
            (long) numBytes, claddrStr, ntohs(claddr.sin6_port));
      
      for (i = 0; i < numBytes; i++)
         buf[i] = toupper((unsigned char) buf[i]);
      
      if (sendto(sfd, buf, numBytes, 0, (struct sockaddr *) &claddr, len) !=
            numBytes)
         usage_error("Partial sento");
   }
}