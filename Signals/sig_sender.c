/* Sending multiiple signals 

   The first argument is the process ID of a process to which the program
   should send signals. The second argument specifies the number of signals
   to be sent to the target process. The third argument specifies the 
   signal number that is to be sent to the target process.
*/

#include "./utils.h"

void main(int argc, char *argv[])
{
   int numSigs, sig, i;
   pid_t pid;

   if (argc < 4 || !strcmp(argv[1], "--help"))
      raiseError("wrong command-line arguments");

   pid = atol(argv[1]);
   numSigs = atoi(argv[2]); 
   sig = atoi(argv[3]);

   /* Send signals to receiver */

   printf("%s: sending signal %d to process %ld %d times\n",
          argv[0], sig, (long) pid, numSigs);

   for (i=0; i<numSigs; i++)
      if (kill(pid, sig) == -1)
         raiseError("kill()");

   /* If a fourth command-line argument was specified, send that signal */

   if (argc > 4)
      if (kill(pid, atoi(argv[4])) == -1)
         raiseError("kill()");

   printf("%s: exiting\n", argv[0]);

   exit(EXIT_SUCCESS);
}