/* Using 'sem_wait()' to decrement a POSIX semaphore */

#include "./utils.h"

void main(int argc, char *argv[])
{
   sem_t *sem;

   if (argc != 2)
      raiseError("wrong command-line arguments");

   sem = sem_open(argv[1], 0);
   if (sem == SEM_FAILED)
      raiseError("sem_open()");

   if (sem_post(sem) == -1)
      raiseError("sem_wait()");

   exit(EXIT_SUCCESS);
}
