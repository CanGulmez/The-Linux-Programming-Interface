/* A file server using System V message queues */

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

static void grimReaper(int sig)
{
   int savedErrno;

   savedErrno = errno;
   while (waitpid(-1, NULL, WNOHANG) > 0)
      continue;
   errno = savedErrno;
}

/* Executed in child process: serve a signle client */
static void serveRequest(const struct requestMsg *req)
{
   int fd;
   ssize_t numRead;
   struct responseMsg resp;

   fd = open(req->pathname, O_RDONLY);
   if (fd == -1) {                  /* Open failed: send error text */
      resp.mtype = RESP_MT_FAILURE;
      snprintf(resp.data, sizeof(resp.data), "%s", "Couldn't open");
      msgsnd(req->clientId, &resp, strlen(resp.data) + 1, 0);
      exit(EXIT_FAILURE);
   }

   resp.mtype = RESP_MT_DATA;
   while ((numRead = read(fd, resp.data, RESP_MSG_SIZE)) > 0)  
      if (msgsnd(req->clientId, &resp, numRead, 0) == -1)
         break;

   /* Send a message of type RESP_MT_END to signify end-to-file */

   resp.mtype = RESP_MT_END;
   msgsnd(req->clientId, &resp, 0, 0); /* Zero-length mtext */
}

void main(int argc, char *argv[])
{
   struct requestMsg req;
   pid_t pid;
   ssize_t msgLen;
   int serverId;
   struct sigaction sa;

   /* Create server message queue */

   serverId = msgget(SERVER_KEY, IPC_CREAT | IPC_EXCL | S_IRUSR |
                     S_IWUSR | S_IWGRP);
   if (serverId == -1)
      syscall_error();

   /* Establish SIGCHLD handler to reap terminated children */

   sigemptyset(&sa.sa_mask);
   sa.sa_flags = SA_RESTART;
   sa.sa_handler = grimReaper;
   if (sigaction(SIGCHLD, &sa, NULL) == -1)
      syscall_error();

   /* Read requests, handle each in a separate child process */

   for (;;) {
      msgLen = msgrcv(serverId, &req, REQ_MSG_SIZE, 0, 0);
      if (msgLen == -1) {
         if (errno == EINTR)     /* Interrupted by SIGCHLD handler? */
            continue;            /* ... then restart msgrcv() */
         syscall_error();        /* Some other error */
         break;                  /* ... so terminate loop */
      }

      pid = fork();              /* Create child process */
      if (pid == -1) {
         syscall_error();
         break;
      }

      if (pid == 0) {            /* Child handles request */
         serveRequest(&req);
         _exit(EXIT_SUCCESS);
      }   

      /* Parent loops to receive next client request */
   }

   /* If msgrcv() or fork() fails, remove server MQ and exit */

   if (msgctl(serverId, IPC_RMID, NULL) == -1)
      syscall_error();

   exit(EXIT_SUCCESS);
}
