/* The program shows the command-line argument's 'dirname()' and 
   'basename()'. 
*/

#include "./utils.h"

void main(int argc, char *argv[])
{
   char *t1, *t2;
   int i;

   for (i=0; i<argc; i++)
   {
      t1 = strdup(argv[i]);
      assert (t1 != NULL);

      t2 = strdup(argv[i]);
      assert (t2 != NULL);

      printf("%s --> %s + %s\n", argv[i], dirname(t1), basename(t2));

      free(t1);
      free(t2);
   }

   exit(EXIT_SUCCESS);
}
