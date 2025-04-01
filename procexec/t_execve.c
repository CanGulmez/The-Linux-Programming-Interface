/* Using 'execve()' to execute a new program */

#include "../linux.h"

void main(int argc, char *argv[])
{
   char *argVec[10];
   char *envVec[] = {"GREET=salut", "BYE=adieu", NULL};

   if (argc != 2 || strcmp(argv[1], "--help") == 0)
      usage_error("Wrong command-line usage");

   argVec[0] = strrchr(argv[1], '/'); /* Get basename from argv[1] */
   if (argVec[0] != NULL)
      argVec[0]++;
   else
      argVec[0] = argv[1];
   argVec[1] = "hello world";
   argVec[2] = "goodbye";
   argVec[3] = NULL;       /* List must be NULL-terminated */
   
   execve(argv[1], argVec, envVec);
   syscall_error();        /* If we get here, something went wrong */
}
