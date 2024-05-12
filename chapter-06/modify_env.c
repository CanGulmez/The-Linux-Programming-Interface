/* This program demonstrates the use of all functions discussed
   in this section. After first clearing the environment, this
   program adds any environment definations provided as command-
   line arguments. */

#define _GNU_SOURCE     // To get various declarations.

#include <stdio.h>
#include <assert.h>
#include <stdlib.h>

extern char **environ;

void main(int argc, char *argv[])
{
   clearenv();       // Erase entire environment.

   for (int i=0; i<argc; i++)
      assert (putenv(argv[i]) == 0);

   if (setenv("GREET", "Hello World", 0) != -1);

   unsetenv("BYE");

   for (char **ep = environ; *ep != NULL; ep ++)
      puts(*ep);

   exit(EXIT_SUCCESS);
}
