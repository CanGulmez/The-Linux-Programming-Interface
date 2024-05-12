/* This program demonstrates the use of 'sysconf()' to display 
   various system limits. */

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

/* Print 'msg' plus sysconf() value for 'name'. */
static void sysconfPrint(const char *msg, int name)
{
   long lim;

   errno = 0;
   lim = sysconf(name);
   if (lim != -1)          // Call succeeded, limit determinate
      printf("%s %ld\n", msg, lim);
   else                    
      if (errno == 0)     // Call succeeded, limit indeterminate
         printf("%s (indeterminate)\n", msg);
      else
         abort();
}

void main(int argc, char *argv[])
{  
   sysconfPrint("_SC_ARG_MAX       :", _SC_ARG_MAX);
   sysconfPrint("_SC_LOGIN_NAME_MAX:", _SC_LOGIN_NAME_MAX);
   sysconfPrint("_SC_OPEN_MAX      :", _SC_OPEN_MAX);
   sysconfPrint("_SC_NGROUPS_MAX   :", _SC_NGROUPS_MAX);
   sysconfPrint("_SC_PAGESIZE      :", _SC_PAGESIZE);
   sysconfPrint("_SC_RTSIG_MAX     :", _SC_RTSIG_MAX);

   exit(EXIT_SUCCESS);
}
