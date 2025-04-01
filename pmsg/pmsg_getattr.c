/* Retrieving POSIX message queue attributes */

#include "../linux.h"

void main(int argc, char *argv[])
{
   mqd_t mqd;
   struct mq_attr attr;

   if (argc != 2 || strcmp(argv[1], "--help") == 0)
      usage_error("Wrong command-line usage");

   mqd = mq_open(argv[1], O_RDONLY);
   if (mqd == (mqd_t) -1)
      syscall_error();

   if (mq_getattr(mqd, &attr) == -1)
      syscall_error();

   printf("Maximum number of messages: %ld\n", attr.mq_maxmsg);
   printf("Maximum message size:       %ld\n", attr.mq_msgsize);
   printf("Current number of messages: %ld\n", attr.mq_curmsgs);
   
   exit(EXIT_SUCCESS);
}
