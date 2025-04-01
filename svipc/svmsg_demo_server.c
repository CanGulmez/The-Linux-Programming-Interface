/* Cleanup of IPC objects within a server */

#include "../linux.h"

#define KEY_FILE     "/tmp/svmsg_file"

void main(int argc, char *argv[])
{
   int msqid;
   key_t key;
   const int MQ_PERMS = S_IRUSR | S_IWUSR | S_IWGRP; /* rw--w---- */

   /* Optional code here to check if another server process is
      already running */

   /* Generate the key for the message queue */

   key = ftok(KEY_FILE, 1);
   if (key == -1)
      syscall_error();

   /* While msgget() fails, try creating the queue exclusively */
 
   while ((msqid = msgget(key, IPC_CREAT | IPC_EXCL | MQ_PERMS)) == -1) {
      if (errno == EEXIST) {  /* MQ with the same key already 
                                 exists - remove it and try again  */
         msqid = msgget(key, 0);
         if (msqid == -1)
            syscall_error();
         if (msgctl(msqid, IPC_RMID, NULL) == -1)
            syscall_error();
         printf("Removed old message queue (id=%d)\n", msqid);
      } else {
         syscall_error();
      }
   }

   /* Upon loop exit, we've successfully created the message queue,
      and we can then carry on to do other work... */

   exit(EXIT_SUCCESS);
}
