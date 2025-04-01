/* Passing the caller's environment to the new program using 'execl()' */

#include "../linux.h"

void main(int argc, char *argv[])
{
   printf("Initial value of USER: %s\n", getenv("USER"));
   if (putenv("USER=britta") != 0)
      syscall_error();

   execl("/usr/bin/printenv", "printenv", "USER", "SHELL", (char *) NULL);

   syscall_error();        /* If we get here, something went wrong */ 
}
