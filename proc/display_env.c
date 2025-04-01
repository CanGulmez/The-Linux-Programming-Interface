/* Displaying the process environment */

#include "../linux.h"

extern char **environ;

void main(int argc, char *argv[])
{
   char **ep;

   for (ep = environ; *ep != NULL; ep++)
      puts(*ep);

   exit(EXIT_SUCCESS);
}
