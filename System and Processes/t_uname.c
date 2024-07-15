/* The program displays the information returned by 'uname()'. */

#include "./utils.h"

void main(int argc, char *argv[])
{
   struct utsname uts;

   if (uname(&uts) == -1)
      raiseError("uname() did not work");

   printf("Node name:   %s\n", uts.nodename);
   printf("System name: %s\n", uts.sysname);
   printf("Release:     %s\n", uts.release);
   printf("Version:     %s\n", uts.version);
   printf("Machine:     %s\n", uts.machine);
   printf("Domain name: %s\n", uts.domainname);

   exit(EXIT_SUCCESS);
}
