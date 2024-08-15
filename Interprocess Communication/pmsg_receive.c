/* Reading a message from a POSIX message queue */

#include "./utils.h"

void usageError(const char *progName)
{
   fprintf(stderr, "Usage: %s [-n] name msg [prio]\n", progName);
   fprintf(stderr, "    -n    Use O_NONBLOCK flag\n");
   exit(EXIT_FAILURE);
}

void main(int argc, char *argv[])
{
   int flags, opt;
   mqd_t mqd;
   unsigned int prio;
   void *buffer;
   struct mq_attr attr;
   ssize_t numRead;

   flags = O_RDONLY;
   while ((opt = getopt(argc, argv, "n")) != -1) {
      switch (opt) {
         case 'n':      flags |= O_NONBLOCK; break;
         default:       usageError(argv[0]);
      }
   }

   if (optind >= argc)
      usageError(argv[0]);

   mqd = mq_open(argv[optind], flags);
   if (mqd == (mqd_t) -1)
      raiseError("mq_open()");

   if (mq_getattr(mqd, &attr) == -1)
      raiseError("mq_getattr()");

   buffer = malloc(attr.mq_msgsize);
   if (buffer == NULL)
      raiseError("malloc()");

   numRead = mq_receive(mqd, buffer, attr.mq_msgsize, &prio);
   if (numRead == -1)
      raiseError("mq_receive");

   printf("Read %ld bytes; priority = %u\n", (long) numRead, prio);
   if (write(STDOUT_FILENO, buffer, numRead) == -1)
      raiseError("write()");

   write(STDOUT_FILENO, "\n", 1);

   exit(EXIT_SUCCESS);
}
