/* Using 'sem_getvalue()' to retrieve the value of a POSIX semaphore */

#include "../linux.h"

void main(int argc, char *argv[])
{
   int value;
   sem_t *sem;

   if (argc != 2)
      usage_error("Wrong command-line usage");

   sem = sem_open(argv[1], 0);
   if (sem == SEM_FAILED)
      syscall_error();

   if (sem_getvalue(sem, &value) == -1)
      syscall_error();

   printf("%d\n", value);
   exit(EXIT_SUCCESS);
}
