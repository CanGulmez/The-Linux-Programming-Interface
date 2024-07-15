/* The program echoes its command-line arguments, one per line of
   output, preceded by a string showing which element of 'argv'
   is being displayed. 
*/

#include "./utils.h"

void main(int argc, char *argv[])
{
   int i;

   for (i=0; i<argc; i++)
      printf("argv[%d] = %s\n", i, argv[i]); 

   exit(EXIT_SUCCESS);
}
 