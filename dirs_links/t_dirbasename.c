/* Using 'dirname()' and 'basename()' */

#include "../linux.h"

void main(int argc, char *argv[])
{
   char *t1, *t2;
   int i;

   for (i = 1; i < argc; i++) {
      t1 = strdup(argv[i]);
      if (t1 == NULL)
         syscall_error();
      t2 = strdup(argv[i]);
      if (t2 == NULL)
         syscall_error();

      printf("%s ==> %s, %s\n", argv[i], dirname(t1), basename(t2));

      free(t1);
      free(t2);
   }

   exit(EXIT_SUCCESS);
}
