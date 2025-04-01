/* Using 'sem_unlink()' to unlink a POSIX named semaphore */

#include "../linux.h"

void main(int argc, char *argv[])
{
   if (argc != 2 || strcmp(argv[1], "--help") == 0)
      usage_error("Wrong command-line usage");

   if (sem_unlink(argv[1]) == -1)
      syscall_error();

   exit(EXIT_SUCCESS);
}
