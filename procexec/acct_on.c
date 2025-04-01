/* Turning process accounting on and off */

#include "../linux.h"

void main(int argc, char *argv[])
{
   if (argc > 2 || (argc > 1 && strcmp(argv[1], "--help") == 0))
      usage_error("Wrong command-line usage");

   if (acct(argv[1]) == -1)
      syscall_error();

   printf("Process accounting %s\n",
      (argv[1] == NULL) ? "disabled" : "enabled");
   exit(EXIT_SUCCESS);
}
