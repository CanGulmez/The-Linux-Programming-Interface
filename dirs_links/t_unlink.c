/* Removing a link with 'unlink()' */

#include "../linux.h"

#define CMD_SIZE  200
#define BUF_SIZE  1024

void main(int argc, char *argv[])
{
   int fd, i, numBlocks;
   char shellCmd[CMD_SIZE];
   char buf[BUF_SIZE];

   if (argc < 2 || strcmp(argv[1], "--help") == 0)
      usage_error("Wrong command-line usage");

   numBlocks = (argc > 2) ? atoi(argv[2]) : 100000;

   fd = open(argv[1], O_WRONLY | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);
   if (fd == -1)
      syscall_error();

   if (unlink(argv[1]) == -1)
      syscall_error();

   for (i = 0; i < numBlocks; i++)
      if (write(fd, buf, BUF_SIZE) != BUF_SIZE)
         syscall_error();

   snprintf(shellCmd, CMD_SIZE, "df -k `dirname %s`", argv[1]);
   system(shellCmd);

   if (close(fd) == -1)
      syscall_error();
   printf("********* Closed file descriptor *********\n");

   system(shellCmd);

   exit(EXIT_SUCCESS);
}
