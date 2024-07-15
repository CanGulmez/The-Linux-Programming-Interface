/* Modifying the process environment */

#include "./utils.h"

extern char **environ;

void main(int argc, char *argv[])
{
   int i;
   char **ep;

   clearenv();       // Erase entire environment

   for (i=1; i<argc; i++)
      if (putenv(argv[i]) != 0)
         raiseError("putenv() did not work");

   if (setenv("GREET", "Hello World", 0) == -1)
      raiseError("setenv() did not work");

   unsetenv("BYE");

   for (ep=environ; *ep != NULL; ep ++)
      printf("%s\n", *ep);

   exit(EXIT_SUCCESS);
}
