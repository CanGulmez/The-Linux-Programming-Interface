/* Implementation of 'ptyFork()' */

#include "../linux.h"

#define MAX_SNAME       1000

int ptyMasterOpen(char *slaveName, size_t snLen)
{
   int masterFd, savedErrno;
   char *p;

   masterFd = posix_openpt(O_RDWR | O_NOCTTY);  /* Open pty master */
   if (masterFd == -1)
      return -1;

   if (grantpt(masterFd) == -1) {               /* Grant access to slave pty */
      savedErrno = errno;
      close(masterFd);                          /* Might change 'errno' */
      errno = savedErrno;     
      return -1;
   }                                      

   if (unlockpt(masterFd) == -1) {              /* Unlock slave pty */
      savedErrno = errno;
      close(masterFd);                          /* Might change 'errno' */
      errno = savedErrno;
      return -1;
   }  

   p = ptsname(masterFd);                       /* Get slave pty name */
   if (p == NULL) {
      savedErrno = errno;
      close(masterFd);                          /* Might change 'errno' */
      errno = savedErrno;
      return -1; 
   }

   if (strlen(p) < snLen) {
      strncpy(slaveName, p, snLen);
   } else {                                     /* Return an error if buffer too small */
      close(masterFd);
      errno = EOVERFLOW;
      return -1;
   }
   
   return masterFd;
}

pid_t ptyFork(int *masterFd, char *slaveName, size_t snLen,
              const struct termios *slaveTermios, 
              const struct winsize *slaveWS)
{
   int mfd, slaveFd, savedErrno;
   pid_t childPid;
   char slname[MAX_SNAME];

   mfd = ptyMasterOpen(slname, MAX_SNAME);
   if (mfd == -1)
      return -1;

   if (slaveName != NULL) {         /* Return slave name to caller */
      if (strlen(slname) < snLen) {
         strncpy(slaveName, slname, snLen);
      } else {                      /* 'slaveName' was too small */
         close(mfd);
         errno = EOVERFLOW;
         return -1;
      }      
   }

   childPid = fork();

   if (childPid == -1) {            /* fork() failed */
      savedErrno = errno;           /* close() might change 'errno' */
      close(mfd);                   /* Don't leak file descriptors */
      errno = savedErrno;
      return -1;
   }

   if (childPid != 0) {             /* Parent */
      *masterFd = mfd;              /* Only parent gets master fd */
      return childPid;              /* Like parent of fork() */
   }

   /* Child falls through to here */

   if (setsid() == -1)               /* Start a new session */
      syscall_error();

   close(mfd);                       /* Not needed in child */

   slaveFd = open(slname, O_RDWR);   /* Becomes controlling tty */
   if (slaveFd == -1)
      syscall_error();

#ifdef TIOCSCTTY                    /* Acquire controlling tty on BSD */
   if (ioctl(slaveFd, TIOCSCTTY, 0) == -1)
      syscall_error();
#endif

   if (slaveTermios != NULL)        /* Set slave tty attributes */
      if (tcsetattr(slaveFd, TCSANOW, slaveTermios) == -1)
         syscall_error();

   if (slaveWS != NULL)             /* Set slave tty window size */
      if (ioctl(slaveFd, TIOCSWINSZ, slaveWS) == -1)
         syscall_error();

   /* Duplicate pty slave to be child's stdin, stdout, and stderr */

   if (dup2(slaveFd, STDIN_FILENO) != STDIN_FILENO)
      syscall_error();
   if (dup2(slaveFd, STDOUT_FILENO) != STDOUT_FILENO)
      syscall_error();
   if (dup2(slaveFd, STDERR_FILENO) != STDERR_FILENO)
      syscall_error();
   if (slaveFd > STDERR_FILENO)     /* Safety check */
      close(slaveFd);               /* No longer need this fd */
   
   return 0;                        /* Like child of fork() */
}
