/* Using exit handlers */

#include "./utils.h"

void atexitFunc1(void)
{
   printf("atexit function 1 called\n");
}

void atexitFunc2(void)
{
   printf("atexit function 2 called\n");
}

void onexitFunc(int exitStatus, void *arg)
{
   printf("on_exit function called: status=%d, arg=%ld\n", 
          exitStatus, (long) arg);
}

void main(int argc, char *argv[])
{
   if (on_exit(onexitFunc, (void *) 10) != 0)
      raiseError("on_exit()");
   if (atexit(atexitFunc1) != 0)
      raiseError("atexit()");
   if (atexit(atexitFunc2) != 0)
      raiseError("atexit()");

   exit(EXIT_SUCCESS);
}
