/* Client for the sequence-number server */

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

static char clientFifo[CLIENT_FIFO_NAME_LEN];

static void removeFifo(void)
{
   unlink(clientFifo);
}

void main(int argc, char *argv[])
{
   int serverFd, clientFd;
   struct request req;
   struct response resp;

   if (argc > 1 && strcmp(argv[1], "--help") == 0)
      usage_error("Wrong command-line usage");

   /* Create our FIFO (before sending request, to avoid a race) */

   umask(0);                  /* So we get the permissions we want */
   snprintf(clientFifo, CLIENT_FIFO_NAME_LEN, CLIENT_FIFO_TEMPLATE,
            (long) getpid());
   if (mkfifo(clientFifo, S_IRUSR | S_IWUSR | S_IWGRP) == -1
       && ERRNO != EEXIST)
      syscall_error();

   if (atexit(removeFifo) != 0)
      syscall_error();

   /* Construct request message, open server FIFO, and send request */

   req.pid = getpid();
   req.seqLen = (argc > 1) ? atoi(argv[1]) : 1;

   serverFd = open(SERVER_FIFO, O_WRONLY);
   if (serverFd == -1)
      syscall_error();

   if (write(serverFd, &req, sizeof(struct request)) != 
       sizeof(struct request))
      syscall_error();
   
   /* Open our FIFO, read and display response */

   clientFd = open(clientFifo, O_RDONLY);
   if (clientFd == -1)
      syscall_error();

   if (read(clientFd, &resp, sizeof(struct response)) != 
       sizeof(struct response))
      syscall_error();

   printf("%d\n", resp.seqNum);
   exit(EXIT_SUCCESS);
}
