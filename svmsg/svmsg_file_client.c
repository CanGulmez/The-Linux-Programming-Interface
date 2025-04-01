/* CLient for file server using System V message queues */

#include "../linux.h"

#define SERVER_KEY      0x1aaaaaa1  /* Key for server's message queue */

struct requestMsg {                 /* Requests (client to server) */
   long mtype;                      /* Unused */
   int clientId;                    /* ID of client's message queue */
   char pathname[PATH_MAX];         /* File to be returned */
};

/* REQ_MSG_SIZE computes size of 'mtext' part of 'requestMsg' structure.
   We use offsetof() to handle the possibility that there are padding
   bytes between the 'clientId' and 'pathname' fields. */

#define REQ_MSG_SIZE    (offsetof(struct requestMsg, pathname) - \
                         offsetof(struct requestMsg, clientId) + PATH_MAX)
#define RESP_MSG_SIZE   8192

struct responseMsg {                /* Responses (server to client) */
   long mtype;                      /* One of RESP_MT_* values below */
   char data[RESP_MSG_SIZE];        /* File content / response message */
};

/* Types for response messages send from server to client */

#define RESP_MT_FAILURE 1           /* File couldn't be opened */
#define RESP_MT_DATA    2           /* Message contains file data */
#define RESP_MT_END     3           /* File data complete */

static int clientId;

static void removeQueue(void)
{
   if (msgctl(clientId, IPC_RMID, NULL) == -1)
      syscall_error();
}

void main(int argc, char *argv[])
{
   struct requestMsg req;
   struct responseMsg resp;
   int serverId, numMsgs;
   ssize_t msgLen, totBytes;

   if (argc != 2 || strcmp(argv[1], "--help") == 0)
      usage_error("Wrong command-line usage");

   if (strlen(argv[1]) > sizeof(req.pathname) - 1)
      usage_error("pathname too long");

   /* Get server's queue identifier; create queue for response */

   serverId = msgget(SERVER_KEY, S_IWUSR);
   if (serverId == -1)
      syscall_error();

   clientId = msgget(IPC_PRIVATE, S_IRUSR | S_IWUSR | S_IWGRP);
   if (clientId == -1)
      syscall_error();

   if (atexit(removeQueue) != 0)
      syscall_error();

   /* Send message asking for file named in argv[1] */

   req.mtype = 1;
   req.clientId = clientId;
   strncpy(req.pathname, argv[1], sizeof(req.pathname) - 1);
   req.pathname[sizeof(req.pathname) - 1] = '\0';

   if (msgsnd(serverId, &req, REQ_MSG_SIZE, 0) == -1)
      syscall_error();

   /* Get first response, which may be failure notification */

   msgLen = msgrcv(clientId, &resp, RESP_MSG_SIZE, 0, 0);
   if (msgLen == -1)
      syscall_error();

   if (resp.mtype == RESP_MT_FAILURE) {
      printf("%s\n", resp.data);
      if (msgctl(clientId, IPC_RMID, NULL) == -1)
         syscall_error();
      exit(EXIT_FAILURE);
   }

   /* File was opened successfully by server; process messages
      (including the one already received) containing file data */

   totBytes = msgLen;
   for (numMsgs = 1; resp.mtype == RESP_MT_DATA; numMsgs ++) {
      msgLen = msgrcv(clientId, &resp, RESP_MSG_SIZE, 0, 0);
      if (msgLen == -1)
         syscall_error();

      totBytes += msgLen;
   }

   printf("Received %ld bytes (%d messages)\n", (long) totBytes, numMsgs);

   exit(EXIT_SUCCESS);
}
