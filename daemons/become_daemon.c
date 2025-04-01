/* Creating a daemon process */

#include "../linux.h"

#define BD_NO_CHDIR           01    /* Don't chdir("/") */
#define BD_NO_CLOSE_FILES     02    /* Don't close all open files */
#define BD_NO_REOPEN_STD_FDS  04    /* Don't reopen 0, 1, 2 to /dev/null */
#define BD_NO_UMASK0          010   /* Don't do a umask(0) */
#define BD_MAX_CLOSE          8193  /* Maximum file descriptors */

/* Returns 0 on success, -1 on error */
int becomeDaemon(int flags)
{
   int maxfd, fd;

   switch (fork()) {                /* Become background process */
      case -1:  return -1;
      case 0:   break;
      default: _exit(EXIT_SUCCESS); 
   }

   if (setsid() == -1)              /* Become leader of new session */
      return -1;

   switch (fork()) {                /* Ensure we are not session leader */
      case -1: return -1;
      case 0:  break;
      default: _exit(EXIT_SUCCESS);
   }

   if (!(flags & BD_NO_UMASK0))
      umask(0);                     /* Clear file mode creation mask */

   if (!(flags & BD_NO_CHDIR))
      chdir("/");                   /* Change to root directory */

   if (!(flags & BD_NO_CLOSE_FILES)) {
      maxfd = sysconf(_SC_OPEN_MAX);
      if (maxfd == -1)
         maxfd = BD_MAX_CLOSE;
      
      for (fd = 0; fd < maxfd; fd ++)
         close(fd);                 /* Close all open files */
   }

   if (!(flags & BD_NO_REOPEN_STD_FDS)) {
      close(STDIN_FILENO);          /* Reopen standard fd's to /dev/null */

      fd = open("/dev/null", O_RDWR);

      if (fd != STDIN_FILENO)
         return -1;
      if (dup2(STDIN_FILENO, STDOUT_FILENO) != STDOUT_FILENO)
         return -1;
      if (dup2(STDIN_FILENO, STDERR_FILENO) != STDERR_FILENO)
         return -1;
   }
   return 0;
}
