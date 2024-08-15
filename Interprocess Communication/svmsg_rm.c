/* Deleting System V message queues */

#include "./utils.h"

void main(int argc, char *argv[])
{
   int i;

   if (argc > 1 && !strcmp(argv[1], "--help"))
      raiseError("wrong command-line arguments");

   for (i=1; i<argc; i++)
      if (msgctl(atoi(argv[i]), IPC_RMID, NULL) == -1)
         raiseError("msgctl()");

   exit(EXIT_SUCCESS); 
}
