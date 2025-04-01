/* Sharing of file offset and open file status flags between parent and child */

#include "../linux.h"

void main(int argc, char *argv[])
{
   int fd, flags;
   char template[] = "/tmp/testXXXXXX";

   setbuf(stdout, NULL);      /* Disable buffering of stdout */

   fd = mkstemp(template);
   if (fd == -1)
      syscall_error();

   printf("File offset before fork(): %lld\n",
      (long long) lseek(fd, 0, SEEK_CUR));

   flags = fcntl(fd, F_GETFL);
   if (flags == -1)
      syscall_error();
   printf("O_APPEND flag before fork() is %s\n",
      (flags & O_APPEND) ? "on" : "off");

   switch (fork()) {

      case -1:
         syscall_error();

      case 0:           /* Child: change file offset and status flags */
         if (lseek(fd, 100, SEEK_SET) == -1)
            syscall_error();

         flags = fcntl(fd, F_GETFL);   /* Fetch current flags */
         if (flags == -1)
            syscall_error();

         flags |= O_APPEND;            /* Turn O_APPEND on */
         if (fcntl(fd, F_SETFL, flags) == -1)
            syscall_error();

         _exit(EXIT_SUCCESS);

      default:       /* Parent: can see file changes made by child */
         if (wait(NULL) == -1)
            syscall_error();           /* Wait for child exit */
         printf("Child has exited\n");

         printf("File offset in parent: %lld\n",
            (long long) lseek(fd, 0, SEEK_CUR));

         flags = fcntl(fd, F_GETFL);
         if (flags == -1)
            syscall_error();
         printf("O_APPEND flag in parent is: %s\n",
            (flags & O_APPEND) ? "on" : "off");
         
            exit(EXIT_SUCCESS);
   }
}
