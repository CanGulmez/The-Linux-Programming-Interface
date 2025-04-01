/* Echoing command-line arguments */

#include "../linux.h"

void main(int argc, char *argv[])
{
   int i;

   for (i = 0; i < argc; i++)
      printf("argv[%d] = %s\n", i, argv[i]);

   exit(EXIT_SUCCESS);
}
