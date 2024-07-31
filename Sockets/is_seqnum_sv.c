/* An iterative server that uses a stream socket to communicate with clients */

#include "./utils.h"

#define PORT_NUM  "50000"     /* Port number for server */
#define INT_LEN   30          /* Size of string able to hold largest 
                                 integer (including terminating '\n') */
#define BACKLOG   50

ssize_t readLine(int fd, void *buffer, size_t n)
{
   ssize_t numRead;              /* # of bytes fetched by last read() */
   size_t totRead;               /* Total bytes read so far */
   char *buf;
   char ch;

   if (n <= 0 || buffer == NULL)
   {
      errno = EINVAL;
      return -1;
   }

   buf = buffer;                 /* No pointer arithmetic on "void *" */

   totRead = 0;
   for (;;)
   {
      numRead = read(fd, &ch, 1);

      if (numRead == -1)
      {
         if (errno == EINTR)     /* Interrupted --> restart read() */
            continue;
         else
            return -1;           /* Some other error */
      }
      else if (numRead == 0)     /* EOF */
      {
         if (totRead == 0)       /* No bytes read; return 0 */
            return 0;
         else
            break;               /* Some bytes read; add '\0' */
      }
      else {                     /* 'numRead' must be 1 if we get here */
         if (totRead < n - 1)    /* Discard > (n - 1) bytes */
         {
            totRead ++;
            *buf ++ = ch;
         }
         if (ch == '\n')
         break;
      }
   }
   *buf = '\0';
   
   return totRead;
}

void main(int argc, char *argv[])
{
   uint32_t seqNum;
   char reqLenStr[INT_LEN];         /* Lenght of requested sequence */
   char seqNumStr[INT_LEN];         /* Start of granted sequence */
   struct sockaddr_storage claddr;
   int lfd, cfd, optval, reqLen;
   socklen_t addrLen;
   struct addrinfo hints;
   struct addrinfo *result, *rp;
#define ADDRSTRLEN   (NI_MAXHOST + NI_MAXSERV + 10)
   char addrStr[ADDRSTRLEN];
   char host[NI_MAXHOST];
   char service[NI_MAXSERV];

   if (argc > 1 && !strcmp(argv[1], "--help"))
      raiseError("wrong command-line arguments");

   seqNum = (argc > 1) ? atoi(argv[1]) : 0;

   if (signal(SIGPIPE, SIG_IGN) == SIG_ERR)
      raiseError("signal()");

   /* Call getaddrinfo() to obtain a list of addresses that we ca
      try binding to */
   
   memset(&hints, 0, sizeof(struct addrinfo));
   hints.ai_canonname = NULL;
   hints.ai_addr = NULL;
   hints.ai_next = NULL;
   hints.ai_socktype = SOCK_STREAM;
   hints.ai_family = AF_UNSPEC;     /* Allow IPv4 or IPv6 */
   hints.ai_flags = AI_PASSIVE | AI_NUMERICSERV;   
                    /* Wildcard IP addresses; service name is numeric */
   
   if (getaddrinfo(NULL, PORT_NUM, &hints, &result) != 0)
      raiseError("getaddrinfo()");

   /* Walk through returned list until we find an address structure 
      that can be used to successfully create and bind a socket */

   optval = 1;
   for (rp = result; rp != NULL; rp=rp->ai_next)
   {
      lfd = socket(rp->ai_family, rp->ai_socktype, rp->ai_protocol);
      if (lfd == -1)
         continue;               /* On error, try next address */

      if (setsockopt(lfd, SOL_SOCKET, SO_REUSEADDR, &optval, sizeof(optval)) == -1)
         raiseError("setsockopt()");

      if (bind(lfd, rp->ai_addr, rp->ai_addrlen) == 0)
         break;                  /* Success */

      /* bind() failed: close this socket and try next address */

      close(lfd);
   }

   if (rp == NULL)
      raiseError("bind()");

   if (listen(lfd, BACKLOG) == -1)
      raiseError("listen()");

   freeaddrinfo(result);

   for (;;)                         /* Handle clients iteratively */
   {
      /* Accept a client connection, obtaining client's address */

      addrLen = sizeof(struct sockaddr_storage);
      cfd = accept(lfd, (struct sockaddr *) &claddr, &addrLen);
      if (cfd == -1)
      {
         raiseError("accept()");
         continue;
      }

      if (getnameinfo((struct sockaddr *) &claddr, addrLen, host, NI_MAXHOST, 
                      service, NI_MAXSERV, 0) == 0)
         snprintf(addrStr, ADDRSTRLEN, "(%s, %s)", host, service);
      else
         snprintf(addrStr, ADDRSTRLEN, "(?UNKNOWN?)");
         
      printf("Connection from %s\n", addrStr);

      /* Read client request, send sequence number back */

      if (readLine(cfd, reqLenStr, INT_LEN) <= 0)
      {
         close(cfd);
         continue;               /* Failed read; skip request */
      }

      reqLen = atoi(reqLenStr);
      if (reqLen <= 0)           /* Watch for misbehaving clients */
      {
         close(cfd);
         continue;               /* Bad request; skip it */
      }

      snprintf(seqNumStr, INT_LEN, "%d\n", seqNum);
      if (write(cfd, &seqNumStr, strlen(seqNumStr)) != strlen(seqNumStr))
         fprintf(stderr, "Error on write");

      seqNum += reqLen;          /* Update sequence number */

      if (close(cfd) == -1)      /* Close connection */
         raiseError("close()");
   }
}
