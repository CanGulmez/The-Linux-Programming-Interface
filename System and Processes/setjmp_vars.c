/* A demonstration of the interaction of compiler optimization and 
   'longjmp()'. 
*/

#include "./utils.h"

static jmp_buf env;

void doJump(int nvar, int rvar, int vvar)
{
   printf("Inside doJump(); nvar=%d rvar=%d vvar=%d\n", nvar, rvar, vvar);
   longjmp(env, 1);
}

void main(int argc, char *argv[])
{
   int nvar;
   register int rvar;         /* Allocated in register if possible */
   volatile int vvar;         /* See text */

   nvar = 111;
   rvar = 222;
   vvar = 333;

   if (setjmp(env) == 0)      /* Code executed after setjmp() */
   {
      nvar = 777;
      rvar = 888;
      vvar = 999;
      doJump(nvar, rvar, vvar);
   }
   else                       /* Code executed after longjmp() */
      printf("After longjmp(): nvar=%d rvar=%d vvar=%d\n", nvar, rvar, vvar);

   exit(EXIT_SUCCESS);
}
