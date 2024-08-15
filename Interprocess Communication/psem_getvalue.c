/* Using 'sem_getvalue()' to retrieve the value of a POSIX semaphore */

#include "./utils.h"

void main(int argc, char *argv[])
{
   int value;
   sem_t *sem;

   if (argc != 2)
      raiseError("wrong command-line arguments");

   sem = sem_open(argv[1], 0); 
   if (sem == SEM_FAILED)
      raiseError("sem_open()");
   
   if (sem_getvalue(sem, &value) == -1)
      raiseError("sem_getvalue()");
   
   printf("%d\n", value);
   
   exit(EXIT_SUCCESS);
}
