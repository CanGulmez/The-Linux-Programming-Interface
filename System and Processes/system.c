/* Implementation of 'system()' */

#include "./utils.h"

int system(const char *command)
{
   sigset_t blockMask, origMask;
   struct sigaction saIgnore, saOrigQuit, saOrigInt, saDefault;
   pid_t childPid;
   int status, savedError;

   if (command == NULL)          /* Is a shell avaliable? */
      return system(":") == 0;

   sigemptyset(&blockMask);            /* Block SIGCHLD */
   sigaddset(&blockMask, SIGCHLD);
   sigprocmask(SIG_BLOCK, &blockMask, &origMask);

   saIgnore.sa_handler = SIG_IGN;      /* Ignore SIGINT and SIGQUIT */
   saIgnore.sa_flags = 0;
   sigemptyset(&saIgnore.sa_mask);

   sigaction(SIGINT, &saIgnore, &saOrigInt);
   sigaction(SIGQUIT, &saIgnore, &saOrigQuit); 

   switch (childPid = fork())
   {
      case -1:    /* fork() failed */
         status = -1;
         break;            /* Carry on to reset signal attributes */

      case 0:     /* Child: exec command */
         saDefault.sa_handler = SIG_DFL;
         saDefault.sa_flags = 0;
         sigemptyset(&saDefault.sa_mask);

         if (saOrigInt.sa_handler != SIG_IGN)
            sigaction(SIGINT, &saDefault, NULL);
         if (saOrigQuit.sa_handler != SIG_IGN)
            sigaction(SIGQUIT, &saDefault, NULL);

         sigprocmask(SIG_SETMASK, &origMask, NULL);

         execl("/bin/bash", "bash", "-c", command, (char *) NULL);
         _exit(127);                   /* We could not exec the shell */

      default:    /* Parent: wait for our child to terminate */
         while (waitpid(childPid, &status, 0) == -1)
         {
            if (errno != EINTR)        /* Error other than EINTR */
            {
               status = -1;
               break;                  /* So exit loop */
            }
         }
         break;
   }
   /* Unblock SIGCHLD, restore dispositions of SIGINT and SIGQUIT */

   savedError = errno;        /* The following may change 'errno' */

   sigprocmask(SIG_SETMASK, &origMask, NULL);
   sigaction(SIGINT, &saOrigInt, NULL);
   sigaction(SIGQUIT, &saOrigInt, NULL);

   errno = savedError;

   return status;
}
