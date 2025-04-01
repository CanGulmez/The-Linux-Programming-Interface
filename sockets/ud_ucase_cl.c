/* A simple UNIX domain datagram client */

#include "../linux.h"

#define BUF_SIZE         10
#define SV_SOCK_PATH    "/tmp/ud_ucase"

void main(int argc, char *argv[])
{
   struct sockaddr_un svaddr, claddr;
   int sfd, i;
   size_t msgLen;
   ssize_t numBytes;
   char resp[BUF_SIZE];

   if (argc < 2 || strcmp(argv[1], "--help") == 0)
      usage_error("Wrong command-line usage");

   /* Create client socket; bind to unique pathname (based on PID) */

   sfd = socket(AF_UNIX, SOCK_DGRAM, 0);
   if (sfd == -1)
      syscall_error();

   memset(&claddr, 0, sizeof(struct sockaddr_un));
   claddr.sun_family = AF_UNIX;
   snprintf(claddr.sun_path, sizeof(claddr.sun_path), 
      "/tmp/ud_ucase_cl.%ld", (long) getpid());

   if (bind(sfd, (struct sockaddr *) &claddr, sizeof(struct sockaddr_un)) == -1)
      syscall_error();

   /* Construct address of server */

   memset(&svaddr, 0, sizeof(struct sockaddr_un));
   svaddr.sun_family = AF_UNIX;
   strncpy(svaddr.sun_path, SV_SOCK_PATH, sizeof(svaddr.sun_path) - 1);

   /* Send messages to server; echo responses on stdout */

   for (i = 1; i < argc; i++) {

      msgLen = strlen(argv[i]);     /* May be longer than BUF_SIZE */
      if (sendto(sfd, argv[i], msgLen, 0, (struct sockaddr *) &svaddr,
            sizeof(struct sockaddr_un)) != msgLen)
         usage_error("Partial sendto");

      numBytes = recvfrom(sfd, resp, BUF_SIZE, 0, NULL, NULL);
      if (numBytes == -1)
         syscall_error();

      printf("Response %d: %.*s\n", i, (int) numBytes, resp);
   }

   remove(claddr.sun_path);         /* Remove client socket pathname */
   exit(EXIT_SUCCESS);
}
