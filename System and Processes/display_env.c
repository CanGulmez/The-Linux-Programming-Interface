/* The program accesses 'environ' in order to list all of the values 
   in the process environment. 
*/

#include "./utils.h"

extern char **environ;

void main(int argc, char *argv[])
{
   char **ep;

   for (ep = environ; *ep != NULL; ep++)
      printf("%s\n", *ep);

   exit(EXIT_SUCCESS);
}
