/* IPv6 case-conversion client using datagram sockets */

#include "./utils.h"

#define BUF_SIZE     10    /* Maximum size of messages exchanged 
                              between client and server */
#define PORT_NUM     50002 /* Server port number */

void main(int argc, char *argv[])
{
   struct sockaddr_in6 svaddr;
   int sfd, j;
   size_t msgLen;
   ssize_t numBytes;
   char resp[BUF_SIZE];

   if (argc < 3 || strcmp(argv[1], "--help") == 0)
      raiseError("wrong command-line arguments");

   sfd = socket(AF_INET6, SOCK_DGRAM, 0);    /* Create client socket */
   if (sfd == -1)
      raiseError("socket()");

   memset(&svaddr, 0, sizeof(struct sockaddr_in6));
   svaddr.sin6_family = AF_INET6;
   svaddr.sin6_port = htons(PORT_NUM);
   if (inet_pton(AF_INET6, argv[1], &svaddr.sin6_addr) <= 0)
      raiseError("inet_pton()");

   /* Send messages to server; echo responses on stdout */

   for (j = 2; j < argc; j++) 
   {
      msgLen = strlen(argv[j]);

      if (sendto(sfd, argv[j], msgLen, 0, (struct sockaddr *) &svaddr, sizeof(struct sockaddr_in6)) != msgLen)
         raiseError("sendto()");

      numBytes = recvfrom(sfd, resp, BUF_SIZE, 0, NULL, NULL);
      if (numBytes == -1)
         raiseError("recvfrom()");

      printf("Response %d: %.*s\n", j - 1, (int) numBytes, resp);
   }

   exit(EXIT_SUCCESS);
}

