/* Sending multiple signals */

#include "../linux.h"

void main(int argc, char *argv[])
{
   int numSigs, sig, i;
   pid_t pid;

   if (argc < 4 || strcmp(argv[1], "--help") == 0)
      usage_error("Wrong command-line usage");

   pid = atol(argv[1]);
   numSigs = atoi(argv[2]);
   sig = atoi(argv[3]);

   /* Send signals to receiver */

   printf("%s: sending signal %d to process %ld %d times\n",
      argv[0], sig, (long) pid, numSigs);

   for (i = 0; i < numSigs; i++)
      if (kill(pid, sig) == -1)
         syscall_error();

   /* If a fourth command-line argument was specified, send that signal */

   if (argc > 4)
      if (kill(pid, atoi(argv[4])))
         syscall_error();

   printf("%s: exiting\n", argv[0]);
   exit(EXIT_SUCCESS);
}
