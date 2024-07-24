/* Parent and child race to write a message after 'fork()'.

   This program demonstrates this indeterminacy. THe program loops, using
   'fork()' to create multiple children. After each 'fork()', both parent
   and child print a message containing the loop counter value and a 
   string indicating whether the process is the parent or child.
*/

#include "./utils.h"

void main(int argc, char *argv[])
{
   int numChildren, i;
   pid_t childPid;

   if (argc > 1 && !strcmp(argv[1], "--help"))
      raiseError("wrong command-line arguments");

   numChildren = (argc > 1) ? atoi(argv[1]) : 1;

   setbuf(stdout, NULL);            /* Make stdout unbuffered */

   for (i=0; i<numChildren; i++)
   {
      switch (childPid = fork())
      {
         case -1:
            raiseError("fork()");

         case 0:
            printf("%d child\n", i);
            _exit(EXIT_SUCCESS);

         default:
            printf("%d parent\n", i);
            wait(NULL);             /* Wait for child to terminate */
            break;
      }
   }
   exit(EXIT_SUCCESS);
}