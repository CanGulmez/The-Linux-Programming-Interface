/* A client that uses stream sockets */

#include "../linux.h"

#define PORT_NUM     "50000"
#define INT_LEN      30

void main(int argc, char *argv[])
{
   char *reqLenStr;
   char seqNumStr[INT_LEN];
   int cfd;
   ssize_t numRead;
   struct addrinfo hints;
   struct addrinfo *result, *rp;

   if (argc < 2 || strcmp(argv[1], "--help") == 0)
   usage_error("Wrong command-line usage");

/* Call getaddrinfo() to obtain a list of addresses that
   we can try connecting to */

   memset(&hints, 0, sizeof(struct addrinfo));
   hints.ai_canonname = NULL;
   hints.ai_addr = NULL;
   hints.ai_next = NULL;
   hints.ai_family = AF_UNSPEC;        /* Allows IPv4 or IPv6 */
   hints.ai_socktype = SOCK_STREAM;
   hints.ai_flags = AI_NUMERICSERV;

   if (getaddrinfo(argv[1], PORT_NUM, &hints, &result) != 0)
      syscall_error();

   /* Walk through returned list until we find an address structure
      that can be used to successfully connect a socket */

   for (rp = result; rp != NULL; rp = rp->ai_next) {
      cfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
      if (cfd == -1)
         continue;                     /* On error, try next address */

      if (connect(cfd, rp->ai_addr, rp->ai_addrlen) != -1)
         break;                        /* Success */

      /* Connect failed: close this socket and try next address */
      
      close(cfd);
   }

   if (rp == NULL)
      usage_error("Could not connect socket to any address");

   freeaddrinfo(result);
   
   /* Send requested sequence length, with terminating newline */

   reqLenStr = (argc > 2) ? argv[2] : "1";
   if (write(cfd, reqLenStr, strlen(reqLenStr)) != strlen(reqLenStr))
      syscall_error("Partial/failed write (reqLenStr)");
   if (write(cfd, "\n", 1) != 1)
      syscall_error("Partial/failed write (newline)");

   /* Read and display sequence number returned by server */

   numRead = readLine(cfd, seqNumStr, INT_LEN);
   if (numRead == -1)
      syscall_error();

   if (numRead == 0)
      usage_error("Unexpected EOF from server");
      
   printf("Sequence number: %s", seqNumStr);    /* Includes '\n' */
   
   exit(EXIT_SUCCESS);                          /* Closes 'cfd' */
}