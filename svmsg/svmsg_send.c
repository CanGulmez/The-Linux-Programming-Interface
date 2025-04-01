/* Using 'msgsnd()' to send a message */

#include "../linux.h"

#define MAX_MTEXT    1024

struct mbuf {
   long mtype;             /* Message type */
   char mtext[MAX_MTEXT];  /* Message body */
};

/* Print (optional) message, then usage description */
static void usageError(const char *progName, const char *msg)
{
   if (msg != NULL)
      fprintf(stderr, "%s", msg);
   fprintf(stderr, "Usage: %s [-n] msqid msg-type [msg-text]\n", progName);
   fprintf(stderr, "    -n       Use IPC_NOWAIT flag\n");
   exit(EXIT_SUCCESS);
}

void main(int argc, char *argv[])
{
   int msqid, flags, msgLen;
   struct mbuf msg;
   int opt;

   /* Parse command-line options and arguments */

   flags = 0;
   while ((opt = getopt(argc, argv, "n")) != -1) {
      if (opt == 'n')
         flags |= IPC_NOWAIT;
      else
         usageError(argv[0], NULL);
   }

   if (argc < optind + 2 || argc > optind + 3)
      usageError(argv[0], "Wrong number of arguments\n");

   msqid = atoi(argv[optind]);
   msg.mtype = atoi(argv[optind + 1]);

   if (argc > optind + 2) {
      msgLen = strlen(argv[optind + 2]) + 1;
      if (msgLen > MAX_MTEXT)
         usage_error("msg-text too long\n");

      memcpy(msg.mtext, argv[optind + 2], msgLen);
   } else {
      msgLen = 0;
   }

   /* Send message */

   if (msgsnd(msqid, &msg, msgLen, flags) == -1)
      syscall_error();

   exit(EXIT_SUCCESS);
}
