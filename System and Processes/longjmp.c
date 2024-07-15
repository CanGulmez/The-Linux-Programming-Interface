/* The program demonstrates the use of 'setjmp' and 'longjmp'. This program
   sets up a jump target with an initial call to 'setjmp()'. The subsequent
   switch is the means of detecting whether we have just completed the
   initial return from 'setjmp()' or whether we have arrived back after a 
   'longjmp'.
*/

#include "./utils.h"

static jmp_buf env;

void f2(void)
{
   longjmp(env, 2);
}

void f1(int argc)
{
   if (argc == 1)
      longjmp(env, 1);
   f2();
}

void main(int argc, char *argv[])
{
   switch (setjmp(env))
   {
      case 0:           /* This is the return after initial setjmp() */
         printf("Calling f1() after initial setjmp()\n");
         f1(argc);      /* Never returns... */
         break;         /* ... but this is good form */

      case 1:
         printf("We jumped back from f1()\n");
         break;

      case 2:
         printf("We jumped back from f2()\n");
         break;
   }

   exit(EXIT_SUCCESS);
}
