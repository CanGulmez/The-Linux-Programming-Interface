/* Using 'mq_unlink()' to unlink a POSIX message queue */

#include  "./utils.h"

void main(int argc, char *argv[])
{
   if (argc != 2 || !strcmp(argv[1], "--help"))
      raiseError("wrong command-line arguments");

   if (mq_unlink(argv[1]) == -1)
      raiseError("mq_unlink()");

   exit(EXIT_SUCCESS);
}
