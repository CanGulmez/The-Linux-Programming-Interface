/* Displaying the contents of a 'utmpx'-format file */

#include "./utils.h"

void main(int argc, char *argv[])
{
   struct utmpx *ut;

   if (argc > 1 && !strcmp(argv[1], "--help"))
      raiseError("wrong command line");

   if (argc > 1)              /* Use alternate file if supplied */
      if (utmpxname(argv[1]) == -1)
         raiseError("utmpxname()");

   
}