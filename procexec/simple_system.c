/* An implementation of 'system()' that excludes signal handling */

#include "../linux.h"

int simple_system(char *command)
{
   int status;
   pid_t childPid;

   switch (childPid == fork()) {
      case -1:
         syscall_error();
      case 0:
         execl("/bin/sh", "sh", "-c", command, (char *) NULL);
         _exit(127);
      default:
         if (waitpid(childPid, &status, 0) == -1)
            return -1;
         else
            return status;
   }
}