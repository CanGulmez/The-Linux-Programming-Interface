/* Using 'sigqueue()' to send realtime signals */

#include "../linux.h"

void main(int argc, char *argv[])
{
   int sig, numSigs, i, sigData;
   union sigval sv;

   if (argc < 4 || strcmp(argv[1], "--help") == 0)
      usage_error("Wrong command-line usage");

   /* Display our PID and UID, so that they can be compared with
      the corresponding fields of the siginfo_t argument supplied
      to the handler in the receiving process */

   printf("%s: PID is %ld, UID is %ld\n", argv[0], 
      (long) getpid(), (long) getuid());

   sig = atoi(argv[2]);
   sigData = atoi(argv[3]);
   numSigs = (argc > 4) ? atoi(argv[4]) : 1;

   for (i = 0; i < numSigs; i++) {
      sv.sival_int = sigData + i;
      if (sigqueue(atoi(argv[1]), sig, sv) == -1)
         syscall_error();
   }
   exit(EXIT_SUCCESS);
}
