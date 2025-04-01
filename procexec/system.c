/* Implementation of 'system()' */

#include "../linux.h"

int full_system(const char * command)
{
   sigset_t blockMask, origMask;
   struct sigaction saIgnore, saOrigQuit, saOrigInt, saDefault;
   pid_t childPid;
   int status, savedErrno;

   if (command == NULL)       /* Is a shell avaliable? */
      return system(":") == 0;

   sigemptyset(&blockMask);      /* Block SIGCHLD */
   sigaddset(&blockMask, SIGCHLD);
   sigprocmask(SIG_BLOCK, &blockMask, &origMask);

   saIgnore.sa_handler = SIG_IGN;   /* Ignore SIGINT and SIGQUIT */
   saIgnore.sa_flags = 0;
   sigemptyset(&saIgnore.sa_mask);
   sigaction(SIGINT, &saIgnore, &saOrigInt);
   sigaction(SIGQUIT, &saIgnore, &saOrigQuit);

   switch (childPid == fork()) {
      case -1:
         status = -1;
         break;
      case 0:        /* Child: exec command */
         saDefault.sa_handler = SIG_DFL;
         saDefault.sa_flags = 0;
         sigemptyset(&saDefault.sa_mask);

         if (saOrigInt.sa_handler != SIG_IGN)
            sigaction(SIGINT, &saDefault, NULL);
         if (saOrigQuit.sa_handler != SIG_IGN)
            sigaction(SIGQUIT, &saDefault, NULL);

         sigprocmask(SIG_SETMASK, &origMask, NULL);

         execl("/bin/sh", "sh", "-c", command, (char *) NULL);
         _exit(127);
      default:       /* Parent: wait for out child to terminate */
         while (waitpid(childPid, &status, 0) == -1) {
            if (errno == EINTR) {
               status = -1;
               break;
            }
         }
         break;
   }

   /* Unblock SIGCHLD, restore dispositions of SIGINT and SIGQUIT */

   savedErrno = errno;        /* The following may change 'errno' */

   sigprocmask(SIG_SETMASK, &origMask, NULL);
   sigaction(SIGINT, &saOrigInt, NULL);
   sigaction(SIGQUIT, &saOrigQuit, NULL);

   errno = savedErrno;

   return status;
}
