/* Using 'sem_post()' to increment a POSIX semaphore */

#include "../linux.h"

void main(int argc, char *argv[])
{
   sem_t *sem;

   if (argc != 2)
      usage_error("Wrong command-line usage");

   sem = sem_open(argv[1], 0);
   if (sem == SEM_FAILED)
      syscall_error();

   if (sem_post(sem) == -1)
      syscall_error();

   exit(EXIT_SUCCESS);
}
