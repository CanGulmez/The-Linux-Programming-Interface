/* Using 'msgsnd()' to send a message */

#include "./utils.h"

#define MAX_MTEXT       64

struct mbuf {
   long mtype;             /* Message type */
   char mtext[MAX_MTEXT];  /* Message body */
};

/* Print (optional) message, then usage description */
void usageError(char *progName, char *msg)
{
   if (msg != NULL)
      fprintf(stderr, "%s", msg);
   fprintf(stderr, "Usage: %s [-n] msqid msg-type [msg-text]\n", progName);
   fprintf(stderr, "    -n    Use IPC_NOWAIT flag\n");
   exit(EXIT_SUCCESS);  
}
 
void main(int argc, char *argv[])
{
   int msqid, flags, msgLen;        /* Message buffer for msgsnd() */
   struct mbuf msg;                 /* Otion character from getopt() */
   int opt;

   /* Parse command-line options and arguments */

   flags = 0;
   while ((opt = getopt(argc, argv, "n")) != -1) {
      if (opt == 'n')
         flags |= IPC_NOWAIT;
      else
         usageError(argv[0], NULL);
   }

   if (argc < optind + 2  || argc > optind + 3)
      usageError(argv[0], "Wrong number of arguments\n");

   msqid = atoi(argv[optind]);
   msg.mtype = atoi(argv[optind + 1]);

   if (argc > optind + 2) {         /* 'msg-text' was supplied */
      msgLen = strlen(argv[optind + 2]) + 1;
      if (msgLen > MAX_MTEXT)
         raiseError("msg-text too long");

      memcpy(msg.mtext, argv[optind + 2], msgLen);
   } 
   else {                 /* No 'msg-text' ==> zero-lenght msg */
      msgLen = 0;
   }

   /* Send message */

   if (msgsnd(msqid, &msg, msgLen, flags) == -1)
      raiseError("msgsnd()");

   exit(EXIT_SUCCESS);
}
