/* Using 'mq_unlink()' to unlink a POSIX message queue */

#include "../linux.h"

void main(int argc, char *argv[])
{
   if (argc != 2 || strcmp(argv[1], "--help") == 0)   
      usage_error("Wrong command-line usage");

   if (mq_unlink(argv[1]) == -1)
      syscall_error();
   exit(EXIT_SUCCESS);
}
