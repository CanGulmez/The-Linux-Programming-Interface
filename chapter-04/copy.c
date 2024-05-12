/* This program is a simple version of the 'cp(1)' command.
   It copies the contents of the existing file named in its
   first command-line argument to the new file named in its
   second command-line argument. */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>

#ifndef BUF_SIZE        // Allow 'cc -D' to override defination.
#define BUF_SIZE  1024
#endif

void main(int argc, char *argv[])
{
   char buffer[50];

   int inputFd, outputFd, openFlags;
   mode_t filePerms;
   ssize_t numRead;
   char buf[BUF_SIZE]; 

   assert (argc == 3 && strcmp(argv[1], "--help"));

   // Open input and output files.

   inputFd = open(argv[1], O_RDONLY);
   assert (inputFd != -1);
   
   openFlags = O_CREAT | O_WRONLY | O_TRUNC;
   filePerms = S_IRUSR | S_IRUSR | S_IRGRP | S_IWGRP |
               S_IWOTH | S_IROTH;      // rw-rw-rw-

   outputFd = open(argv[2], openFlags, filePerms);
   assert (outputFd != -1);

   // Transfer data until we encounter end of input or an error.

   while ((numRead = read(inputFd, buf, BUF_SIZE)) > 0)
      assert (write(outputFd, buf, numRead) == numRead);

   assert (numRead != -1);

   assert (close(inputFd) != -1);
   assert (close(outputFd) != -1);

   exit(EXIT_SUCCESS);
}
