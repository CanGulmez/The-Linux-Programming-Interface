/* Using 'sem_unlink()' to unlink a POSIX named semaphore */

#include "./utils.h"

void main(int argc, char *argv[])
{
   if (argc != 2 || !strcmp(argv[1], "--help"))
      raiseError("wrong command-line arguments");

   if (sem_unlink(argv[1]) == -1)
      raiseError("sem_unlink()");

   exit(EXIT_SUCCESS);
}
