/* Displaying all System V message queues on the system */

#include "../linux.h"

void main(int argc, char *argv[])
{
   int maxind, ind, msqid;
   struct msqid_ds ds;
   struct msginfo msginfo;

   /* Obtain size of kernel 'entries' array */

   maxind = msgctl(0, MSG_INFO, (struct msqid_ds *) &msginfo);
   if (maxind == -1)
      syscall_error();

   printf("maxind: %d\n\n", maxind);
   printf("index\t\tid\t\tkey\t\tmessages\n");

   /* Retrieve and display information from each element of 'entries' array */

   for (ind = 0; ind <= maxind; ind ++) {
      msqid = msgctl(ind, MSG_STAT, &ds);
      if (msqid == -1) {
         if (errno != EINVAL && errno != EACCES)
            syscall_error();
         continue;
      }
      printf("%4d\t%8d\t0x%08lx\t%7ld\n", ind, msqid, 
         (unsigned long) ds.msg_perm.__key, (long) ds.msg_qnum);
   }

   exit(EXIT_SUCCESS);
}
