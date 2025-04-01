/* Modifying the process environment */

#include "../linux.h"

extern char **environ;

void main(int argc, char *argv[])
{
   int i;
   char **ep;

   clearenv();

   for (i = 1; i < argc; i++)
      if (putenv(argv[i]) != 0)
         syscall_error();

   if (setenv("GREET", "Hello World", 0) == -1)
      syscall_error();

   unsetenv("BYE");

   for (ep = environ; *ep != NULL; ep++)
      puts(*ep);

   exit(EXIT_SUCCESS);
}
