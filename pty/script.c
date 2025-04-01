/* A simple implementation of 'script(1)' */

#include "../linux.h"

#define BUF_SIZE     256
#define MAX_SNAME    1000

struct termios ttyOrig;

/* Place terminal referred to by 'fd' in raw mode (noncanonical mode
   with all input and output processing disabled). Return 0 on success,
   or -1 on error. If 'prevTermios' is non-NULL, then use the buffer to
   which it points to return the previous terminal settings. */

int ttySetRaw(int fd, struct termios *prevTermios)
{
   struct termios t;

   if (tcgetattr(fd, &t) == -1)
      return -1;

   if (prevTermios != NULL)
      *prevTermios = t;

   t.c_lflag &= ~(ICANON | ISIG | IEXTEN | ECHO);
                  /* Noncanonical mode, disable signals, extended
                     input processing, and echoing */
   
   t.c_iflag &= ~(BRKINT | ICRNL | IGNBRK | IGNCR | INLCR |
                  INPCK | ISTRIP | IXON | PARMRK);
                  /* Disable special handling of CR, NL, and BREAK.
                     No 8th-bit stripping or parity error handling.
                     Disable START/STOP output flow control. */
   t.c_oflag &= ~OPOST;      /* Disable all output processing */
   t.c_cc[VMIN] = 1;
   t.c_cc[VTIME] = 0;         /* Character-at-a-time input */
                              /* with blocking */
   if (tcsetattr(fd, TCSAFLUSH, &t) == -1)
      return -1;
   
   return 0;
}   

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

/* Reset terminal mode on program exit */

static void ttyReset(void)
{
   if (tcsetattr(STDIN_FILENO, TCSANOW, &ttyOrig) == -1)
      syscall_error();
}

void main(int argc, char *argv[])
{
   char slaveName[MAX_SNAME];
   char *shell;
   int masterFd, scriptFd;
   struct winsize ws;
   fd_set inFds;
   char buf[BUF_SIZE];
   ssize_t numRead;
   pid_t childPid;

   if (tcgetattr(STDIN_FILENO, &ttyOrig) == -1)
      syscall_error();
   if (ioctl(STDIN_FILENO, TIOCGWINSZ, &ws) < 0)
      syscall_error();

   childPid = ptyFork(&masterFd, slaveName, MAX_SNAME, &ttyOrig, &ws);
   if (childPid == -1)
      syscall_error();

   if (childPid == 0) {    /* Child: execute a shell on pty slave */
      shell = getenv("SHELL");
      if (shell == NULL || *shell == '\0')
         shell = "/bin/sh";
      
      execlp(shell, shell, (char *) NULL);
      syscall_error();     /* If we get here, something went wrong */
   }

   /* Parent: relay data between terminal and pty master */

   scriptFd = open((argc > 1) ? argv[1] : "typescript",
      O_WRONLY | O_CREAT | O_TRUNC,
      S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP |
      S_IROTH | S_IWOTH);
   
   if (scriptFd == -1)
      syscall_error();

   ttySetRaw(STDIN_FILENO, &ttyOrig);

   if (atexit(ttyReset) != 0)
      syscall_error();

   for (;;) {
      FD_ZERO(&inFds);
      FD_SET(STDIN_FILENO, &inFds);
      FD_SET(masterFd, &inFds);
      
      if (select(masterFd + 1, &inFds, NULL, NULL, NULL) == -1)
         syscall_error();
      
      if (FD_ISSET(STDIN_FILENO, &inFds)) {     /* stdin --> pty */
         numRead = read(STDIN_FILENO, buf, BUF_SIZE);
         if (numRead <= 0)
            exit(EXIT_SUCCESS);

         if (write(masterFd, buf, numRead) != numRead)
            usage_error("partial/failed write (masterFd)");
      }
      
      if (FD_ISSET(masterFd, &inFds)) {         /* pty --> stdout+file */
         numRead = read(masterFd, buf, BUF_SIZE);
         if (numRead <= 0)
            exit(EXIT_SUCCESS);
      
         if (write(STDOUT_FILENO, buf, numRead) != numRead)
            usage_error("partial/failed write (STDOUT_FILENO)");
         if (write(scriptFd, buf, numRead) != numRead)
            usage_error("partial/failed write (scriptFd)");
      }
   }
}
