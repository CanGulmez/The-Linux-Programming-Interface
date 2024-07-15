/* The program demonstrates the use of 'sysconf' to display various system
   limits. 
*/

#include "./utils.h"
 
/* Print 'msg' plus sysconf() value for 'name' */
void sysconfPrint(const char *msg, int name)
{
   long lim;

   errno = 0;
   lim = sysconf(name);

   if (lim != -1)          /* Call succeeded, limit determine */
      printf("%s %ld\n", msg, lim);
   else
   {
      if (errno == 0)      /* Call succeeded, limit indeeterminate */
         printf("%s (indeterminate)\n", msg);
      else                 /* Call failed */
         raiseError("sysconf() did not work");
   }
} 
 
void main(int argc, char *argv[])
{
   sysconfPrint("_SC_ARG_MAX:        ", _SC_ARG_MAX);
   sysconfPrint("_SC_LOGIN_NAME_MAX: ", _SC_LOGIN_NAME_MAX);
   sysconfPrint("_SC_OPEN_MAX:       ", _SC_OPEN_MAX);
   sysconfPrint("_SC_NGROUPS_MAX:    ", _SC_NGROUPS_MAX);
   sysconfPrint("_SC_PAGE_SIZE:      ", _SC_PAGE_SIZE);
   sysconfPrint("_SC_RTSIG_MAX:      " , _SC_RTSIG_MAX);

   exit(EXIT_SUCCESS);
}
  