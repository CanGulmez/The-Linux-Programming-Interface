/* Changing the 'msg_qbytes' setting of a System V message queue */

#include "../linux.h"

void main(int argc, char *argv[])
{
   struct msqid_ds ds;
   int msqid;

   if (argc != 3 || strcmp(argv[1], "--help") == 0)
      usage_error("Wrong command-line usage");

   /* Retrieve copy of associated data structure from kernel */

   msqid = atoi(argv[1]);
   if (msgctl(msqid, IPC_STAT, &ds) == -1)
      syscall_error();

   ds.msg_qbytes = atoi(argv[2]);

   /* Update associated data structure in kernel */

   if (msgctl(msqid, IPC_SET, &ds) == -1)
      syscall_error();

   exit(EXIT_SUCCESS);
}
 