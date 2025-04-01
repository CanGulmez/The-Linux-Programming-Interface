/* Read and resolve a symbolic link */

#include "../linux.h"

#define BUF_SIZE PATH_MAX

void main(int argc, char *argv[])
{
   struct stat statbuf;
   char buffer[BUF_SIZE];
   ssize_t numBytes;

   if (argc != 2 || strcmp(argv[1], "--help") == 0)
      usage_error("Wrong command-line usage");

   if (lstat(argv[1], &statbuf) == -1)
      syscall_error();

   if (!S_ISLNK(statbuf.st_mode))
      usage_error("Not a symbolic link");

   numBytes = readlink(argv[1], buffer, BUF_SIZE - 1);
   if (numBytes == -1)
      syscall_error();

   buffer[numBytes] = '\0';
   printf("readlink: %s --> %s\n", argv[1], buffer);

   if (realpath(argv[1], buffer) == NULL)
      syscall_error();
   printf("realpath: %s --> %s\n", argv[1], buffer);

   exit(EXIT_SUCCESS);
}