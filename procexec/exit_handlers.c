/* Using exit handlers */

#include "../linux.h"

static void atexitFunc1(void)
{
   printf("atexit function 1 called\n");
}

static void atexitFunc2(void)
{
   printf("atexit function 2 called\n");
}

static void onexitFunc(int exitStatus, void *arg)
{
   printf("on_exit function called: status=%d, arg=%ld\n",
      exitStatus, (long) arg);
}

void main(int argc, char *argv[])
{
   if (on_exit(onexitFunc, (void *) 10) != 0)
      syscall_error();
   if (atexit(atexitFunc1) != 0)
      syscall_error();
   if (atexit(atexitFunc2) != 0)
      syscall_error();
   if (on_exit(onexitFunc, (void *) 20) != 0)
      syscall_error();

   exit(2);
}

