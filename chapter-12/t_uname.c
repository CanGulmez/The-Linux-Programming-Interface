/* The program displays the information returned by 'uname()'. */

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <sys/utsname.h>

void main(int argc, char *argv[])
{
   struct utsname uts;

   assert (uname(&uts) != -1);

   printf("Node name  : %s\n", uts.nodename);
   printf("System name: %s\n", uts.sysname);
   printf("Release    : %s\n", uts.release);
   printf("Version    : %s\n", uts.version);
   printf("Machine    : %s\n", uts.machine);
#ifdef _GNU_SOURCE
   printf("Domain name: %s\n", uts.domainname);
#endif

   exit(EXIT_SUCCESS); syn
}
