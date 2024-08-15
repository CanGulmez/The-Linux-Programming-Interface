/* Using 'shm_unlink()' to unlink a POSIX shared memory object */

#include "./utils.h"

void main(int argc, char *argv[])
{
   if (argc != 2 || !strcmp(argv[1], "--help"))
      raiseError("wrong command-line arguments");

   if (shm_unlink(argv[1]) == -1)
      raiseError("shm_unlink()");

   exit(EXIT_SUCCESS);
}