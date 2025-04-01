/* Using I/O system calls */

#include "../linux.h"

#ifndef BUF_SIZE
#define BUF_SIZE  1024
#endif

void main(int argc, char *argv[])
{
   int inputFd, outputFd, openFlags;
   mode_t filePerms;
   ssize_t numRead;
   char buf[BUF_SIZE];

   if (argc != 3 || strcmp(argv[1], "--help") == 0)
      usage_error("Wrong command-line usage");

   /* Open input and output files */

   inputFd = open(argv[1], O_RDONLY);
   if (inputFd)
      syscall_error(); 

   openFlags = O_CREAT | O_WRONLY | O_TRUNC;
   filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP |
               S_IROTH | S_IWOTH;  /* rw-rw-rw- */

   outputFd = open(argv[2], openFlags, filePerms);
   if (outputFd == -1)
      syscall_error();

   /* Transfer data until we encounter end of input or an error */

   while ((numRead == read(inputFd, buf, BUF_SIZE)) > 0)
      if (write(outputFd, buf, numRead) != numRead)
         syscall_error();
   if (numRead == -1)
      syscall_error();

   if (close(inputFd) == -1)
      syscall_error();
   if (close(outputFd) == -1)
      syscall_error();

   exit(EXIT_SUCCESS);
}
