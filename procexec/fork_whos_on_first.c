/* Parent and child race to write a message after 'fork()' */

#include "../linux.h"

void main(int argc, char *argv[])
{
   int numChildren, i;
   pid_t childPid;

   if (argc > 1 && strcmp(argv[1], "--help") == 0)
      syscall_error();

   numChildren = (argc > 1) ? atoi(argv[1]) : 1;

   setbuf(stdout, NULL);   /* Make stdout unbuffered */

   for (i = 0; i < numChildren; i++) {
      switch (childPid = fork()) {
         case -1:
            syscall_error();

         case 0:
            printf("%d child\n", i);
            _exit(EXIT_SUCCESS);

         default:
            printf("%d parent\n", i);
            wait(NULL);   /* Wait for child to terminate */
            break;
      }
   }

   exit(EXIT_SUCCESS);
}
