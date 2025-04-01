/* Creating a PID lock file to ensure just one instance of a program is started */

#include "../linux.h"

#define BUF_SIZE     100   /* Large enough to hold maximum PID as string */

/* Open/create the file named in 'pidFile', lock it, optionally set the
   close-on-exec flag for the file descriptor, write our PID into the file,
   and (in case the caller is interested) return the file descriptor
   referring to the locked file. The caller is responsible for deleting
   'pidFile' file (just) before process termination. 'progName' should be the
   name of the calling program (i.e., argv[0] or similar), and is used only for
   diagnostic messages. If we can't open 'pidFile', or we encounter some other
   error, then we print an appropriate diagnostic and terminate. */

/* Lock a file region (private; public interfaces below) */

static int lockReg(int fd, int cmd, int type, int whence, int start, off_t len)
{
   struct flock fl;

   fl.l_type = type;
   fl.l_whence = whence;
   fl.l_start = start;
   fl.l_len = len;

   return fcntl(fd, cmd, &fl);
}

/* Lock a file region using using nonblocking F_SETLK */

int lockRegion(int fd, int type, int whence, int start, int len)
{
   return lockReg(fd, F_SETLK, type, whence, start, len);
}

int createPidFile(const char *progName, const char *pidFile, int flags)
{
   int fd;
   char buf[BUF_SIZE];

   fd = open(pidFile, O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
   if (fd == -1)
      syscall_error();

   if (flags & CPF_CLOEXEC) {

      /* Set the close-on-exec file descriptor flag */

      flags = fcntl(fd, F_GETFD);
      if (flags == -1)
         syscall_error();

      flags |= FD_CLOEXEC;

      if (fcntl(fd, F_SETFD, flags) == -1)
         syscall_error();
   }

   if (lockRegion(fd, F_WRLCK, SEEK_SET, 0, 0) == -1) {
      if (errno == EAGAIN || errno == EACCES)
         syscall_error();
      else
         syscall_error();
   }

   if (ftruncate(fd, 0) == -1)
      syscall_error();

   snprintf(buf, BUF_SIZE, "%ld\n", (long) getpid());
   if (write(fd, buf, strlen(buf)) != strlen(buf))
      syscall_error();

   return fd;
}
