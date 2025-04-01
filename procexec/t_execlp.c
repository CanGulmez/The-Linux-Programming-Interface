/* Using 'execlp()' to search for a filename in PATH */

#include "../linux.h"

void main(int argc, char *argv[])
{
   if (argc != 2 || strcmp(argv[1], "--help") == 0)
      usage_error("Wrong command-line usage");

   execlp(argv[1], argv[1], "hello world", (char *) NULL);

   syscall_error();  /* If we get here, something went wrong */
} 
