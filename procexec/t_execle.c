/* Using 'execle()' to specify program arguments as a list */

#include "../linux.h"

void main(int argc, char *argv[])
{
   char *envVec[] = {"GREET=salut", "BYE=adieu", NULL};
   char *filename;

   if (argc != 2 || strcmp(argv[1], "--help") == 0)
      usage_error("Wrong command-line usage");

   filename = strrchr(argv[1], '/');  /* Get basename from argv[1] */
   if (filename != NULL)
      filename ++;
   else
      filename = argv[1];

   execle(argv[1], filename, "hello world", (char *) NULL, envVec);

   syscall_error();                   /* If we get here, something went wrong */
}