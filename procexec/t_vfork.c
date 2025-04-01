/* Using 'vfork()' */

#include "../linux.h"

void main(int argc, char *argv[])
{
   int istack = 222;

   switch (vfork()) {

      case -1:
         syscall_error();

      case 0:
         sleep(3);
         write(STDOUT_FILENO, "Child executing\n", 16);
         istack *= 3;         /* This change will be seen by parent */
         _exit(EXIT_SUCCESS);

      default:
         write(STDOUT_FILENO, "Parent executing\n", 17);
         printf("istack=%d\n", istack);
         exit(EXIT_SUCCESS);
   }
}
