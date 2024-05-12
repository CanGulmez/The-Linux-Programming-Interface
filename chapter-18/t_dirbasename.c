/* Using 'dirname()' and 'basename()' */

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <assert.h>
#include <libgen.h>

void main(int argc, char *argv[])
{
   char *t1, *t2;

   for (int i=0; i<argc; i++)
   {
      t1 = strdup(argv[i]);
      assert (t1 != NULL);
      t2 = strdup(argv[i]);
      assert (t2 != NULL);

      printf("%s ==> %s + %s\n", argv[i], dirname(t1), basename(t2));

      free(t1);
      free(t2);
   }
   exit(EXIT_SUCCESS); 
}
