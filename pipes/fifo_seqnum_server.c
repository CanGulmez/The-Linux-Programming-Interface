/* An iterative server using FIFOs */

#include "../linux.h"

#define SERVER_FIFO           "/tmp/seqnum_sv"  
#define CLIENT_FIFO_TEMPLATE  "/tmp/seqnum_cl.%ld"
#define CLIENT_FIFO_NAME_LEN  (sizeof(CLIENT_FIFO_TEMPLATE) + 20)

struct request {
   pid_t pid;
   int seqLen;
};

struct response {
   int seqNum;
};

void main(int argc, char *argv[])
{
   int serverFd, dummyFd, clientFd;
   char clientFifo[CLIENT_FIFO_NAME_LEN];
   struct request req;
   struct response resp;
   int seqNum = 0;

   /* Create well-known FIFO, and open it for reading */

   umask(0);                  /* So we get the permissions we want */
   if (mkfifo(SERVER_FIFO, S_IRUSR | S_IWUSR | S_IWGRP) == -1
       && errno != EEXIST)
      syscall_error();
   serverFd = open(SERVER_FIFO, O_RDONLY);
   if (serverFd == -1)
      syscall_error();

   /* Open an extra write descriptor, so that we never see EOF */

   dummyFd = open(SERVER_FIFO, O_WRONLY)
   if (dummyFd == -1)
      syscall_error();

   if (signal(SIGPIPE, SIG_IGN) == SIG_ERR)
      syscall_error();

   for (;;) {                 /* Read request and send responses */
      if (read(severFd, &reg, sizeof(struct request)) 
          != sizeof(struct request)) {
         fprintf(stderr, "Error reading request; discarding\n");
         continue;
      }

      /* Open client FIFO(previously created vy client) */

      snprintf(clientFifo, CLIENT_FIFO_NAME_LEN, CLIENT_FIFO_TEMPLATE,
               (long) req.pid);
      clientFd = open(clientFifo, O_WRONLY);
      if (clientFd == -1) {
         syscall_error();
         continue;
      }

      /* Send response and close FIFO */

      resp.seqNum = seqNum;
      if (write(clientFd, &resp, sizeof(struct response)) 
          != sizeof(struct response))
         fprintf(stderr, "Error writing to FIFO %s\n", clientFifo);
      if (close(clientFd) == -1)
         syscall_error();

      seqNum += req.seqLen;      /* Update our sequence number */
   }
}
