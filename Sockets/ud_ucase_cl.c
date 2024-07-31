/* A simple UNIX domain datagram client */

#include "./utils.h"

#define SV_SOCK_PATH "/tmp/ud_ucase"
#define BUF_SIZE     10    /* Maximum size of messages exchanged 
                              between client to server */

void main(int argc, char *argv[])
{
   struct sockaddr_un svaddr, claddr;
   int sfd, i;
   size_t msgLen;
   ssize_t numBytes;
   char buffer[BUF_SIZE];

   if (argc < 2 || !strcmp(argv[1], "--help"))
      raiseError("wrong command-line arguments");

   /* Create client socket; bind to unique pathname (based on PID) */

   sfd = socket(AF_UNIX, SOCK_DGRAM, 0); 
   if (sfd == -1)
      raiseError("socket()");

   memset(&claddr, 0, sizeof (struct sockaddr_un));
   claddr.sun_family = AF_UNIX;
   snprintf(claddr.sun_path, sizeof(claddr.sun_path), "/tmp/ud_ucase_cl.%ld", (long) getpid());
   
   if (bind(sfd, (struct sockaddr *) &claddr, sizeof (struct sockaddr_un)) == -1)
      raiseError("bind()");

   /* Construct address of server */

   memset(&svaddr, 0, sizeof (struct sockaddr_un));
   svaddr.sun_family = AF_UNIX;
   strncpy(svaddr.sun_path, SV_SOCK_PATH, sizeof(svaddr.sun_path) - 1);

   /* Send messages to server; echo responses on stdout */

   for (i=1; i<argc; i++) 
   {
      msgLen = strlen(argv[i]);     /* May be longer than BUF_SIZE */
      if (sendto(sfd, argv[i], msgLen, 0, (struct sockaddr *) &svaddr, 
                 sizeof(struct sockaddr_un)) != msgLen)
         raiseError("sendto()");

      numBytes = recvfrom(sfd, buffer, BUF_SIZE, 0, NULL, NULL);
      if (numBytes == -1)
         raiseError("recvfrom()");
      
      printf("Response %d: %.*s\n", i, (int) numBytes, buffer);
   }
   remove(claddr.sun_path);         /* Remove client socket pathname */
   
   exit(EXIT_SUCCESS);
}

