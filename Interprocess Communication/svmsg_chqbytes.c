/* Changing the 'msg_qbytes' setting of a System V message queue */

#include "./utils.h"

void main(int argc, char *argv[])
{
   struct msqid_ds ds;
   int msqid;

   if (argc != 3 || !strcmp(argv[1], "--help"))
      raiseError("wrong command-line arguments");

   /* Retreive copy of associated data structure from kernel */

   msqid = atoi(argv[1]);
   if (msgctl(msqid, IPC_STAT, &ds) == -1)
      raiseError("msgctl()");

   ds.msg_qbytes = atoi(argv[2]);

   /* Update associated data structure in kernel */

   if (msgctl(msqid, IPC_SET, &ds) == -1)
      raiseError("msgctl()"); 

   exit(EXIT_SUCCESS);
}
 