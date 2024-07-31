/* Using 'sigqueue()' to send realtime signals 

   This program takes up to four arguments, of which the first three are
   mandatory; a signal number, a target process ID, and an integer value to
   accompany the realtime signal. If more than one instance of the specified
   signal is to be sent, the optional fourth arrgument specifies the number
   of instances; in this case, the accompanying integer data value is
   incremented by one for each successive signal.
*/

#include "./utils.h"

void main(int argc, char *argv[])
{
   int sig, numSigs, i, sigData;
   union sigval sv;

   if (argc < 4 || !strcmp(argv[1], "--help"))
      raiseError("wrong command-line arguments");

   /* Display our PID and UID, so that they can be compared with the
      corresponding fields of the siginfo_t argument supplied to the
      handler in the receiving process */

   printf("%s: PID is %ld, UID is %ld\n", argv[0], (long) getpid(), 
          (long) getuid());

   sig, sigData = atoi(argv[2]), atoi(argv[3]);
   numSigs = (argc > 4) ? atoi(argv[4]) : 1;

   for (i=0; i<numSigs; i++)
   {
      sv.sival_int = sigData + i;

      if (sigqueue(atol(argv[1]), sig, sv) == -1)
         raiseError("sigqueue()");
   }

   exit(EXIT_SUCCESS);
}
