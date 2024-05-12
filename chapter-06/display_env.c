/* This program accesses 'environ' in order to list all of the
   values in the process environment. */

#include <stdio.h>
#include <stdlib.h>

extern char **environ

void main(int argc, char *argv[])
[
   char **ep;

   for (ep = environ; *ep != NULL; ep ++)
      puts(*ep);

   exit(EXIT_SUCCESS); 
]
