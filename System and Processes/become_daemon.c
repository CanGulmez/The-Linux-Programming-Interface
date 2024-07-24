/* Creating a daemon process */

#include "./utils.h"

/* Bit-mask values for 'flags' argument of becomeDaemon() */

#define BD_NO_CHIR               01    /* Don't chdir("/") */
#define BD_NO_CLOSE_FILES        02    /* Don't close all open files */
#define BD_NO_REOPEN_STD_FDS     04    /* Don't reopen stdin, stdout, 
                                          and stderr to /dev/null */   
#define BD_NO_UMASKO            010    /* Don't do a mask(0) */

#define BD_MAX_CLOSE 8192    /* Maximum file descriptors to close if 
                                sysconf(_SC_OPEN_MAX) is indeterminate */

/* Return 0 on succes, -1 on error */
int becomeDaemon(int flags)
{
   int maxfd, fd;

   switch (fork())               /* Become background process */
   {
      case -1:    return -1;     /* Child falls through... */
      case 0:     break;         /* while parent terminates */
      default:    _exit(EXIT_SUCCESS);
   }

   if (setsid() == -1)           /* Become leader of new session */
      return -1;

   switch (fork())               /* Ensure we are not session leader */
   {
      case -1:    return -1;
      case 0:     break;
      default:    _exit(EXIT_SUCCESS);
   }

   if (!(flags & BD_NO_UMASKO))
      umask(0);                  /* Clear file mode creation mask */

   if (!(flags & BD_NO_CHIR))
      chdir("/");                /* Change to root directory */

   if (!(flags & BD_NO_CLOSE_FILES))   /* Close all open files */
   {
      maxfd = sysconf(_SC_OPEN_MAX);
      if (maxfd == -1)                 /* Limit is indeterminate */
         maxfd = BD_MAX_CLOSE;         /* so take a guess */

      for (fd=0; fd<maxfd; fd++)
         close(fd);
   }

   if (!(flags & BD_NO_REOPEN_STD_FDS))
   {
      close(STDIN_FILENO);        /* Reopen standard fd's to /dev/null */

      fd = open("/dev/null", O_RDWR);

      if (fd != STDOUT_FILENO)   /* 'fd' should be 0 */
         return -1;
      if (dup2(STDIN_FILENO, STDOUT_FILENO) != STDOUT_FILENO)
         return -1;
      if (dup2(STDIN_FILENO, STDERR_FILENO) != STDERR_FILENO)
         return -1;
   }

   return 0;
}

