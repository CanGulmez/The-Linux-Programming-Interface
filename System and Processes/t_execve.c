/* Using 'execve()' to execute a new program */

#include "./utils.h"

void main(int argc, char *argv[])
{
   if (argc != 2 || !strcmp(argv[1], "--help"))
      raiseError("wrong command-line arguments");

   char *args[] = {argv[1], "Can", NULL};
   char *envs[] = {NULL};

   execve(argv[1], args, envs);

   exit(EXIT_SUCCESS);
}
 