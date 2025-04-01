/* Using 'sem_wait()' to decrement a POSIX semaphore */

#include "../linux.h"

void main(int argc, char *argv[])
{
   sem_t *sem;

   if (argc < 2 || strcmp(argv[1], "--help") == 0)
      usage_error("Wrong command-line usage");

   sem = sem_open(argv[1], 0);
   if (sem == SEM_FAILED)
      syscall_error();

   if (sem_wait(sem) == -1)
      syscall_error();

   printf("%ld sem_wait() succeeded\n", (long) getpid());
   exit(EXIT_SUCCESS);
}
