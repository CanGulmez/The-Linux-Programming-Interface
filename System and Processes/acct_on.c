/* Turning process accounting on and off */

#include "./utils.h"

void main(int argc, char *argv[])
{
   if (argc > 2 || (argc > 1 && !strcmp(argv[1], "--help")))
      raiseError("wrong command-line arguments");

   if (acct(argv[1]) == -1)
      raiseError("acct()");

   printf("Process accounting %s\n", 
          (argv[1] == NULL) ? "disabled" : "enabled");

   exit(EXIT_SUCCESS);   
}
