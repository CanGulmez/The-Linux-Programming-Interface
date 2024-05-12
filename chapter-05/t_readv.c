/* This program demonstrades the use of 'readv()'. */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <fcntl.h>
#include <sys/uio.h>
#include <sys/stat.h>

void main(int argc, char *argv)
{
   int fd;
   struct iovec iov[3];
   struct stat myStruct;      // First buffer.
   int x;                     // Second buffer.
#define STR_SIZE  100
   char str[STR_SIZE];        // Third buffer.

   ssize_t numRead, totRequired; 

   assert (argc == 2 && strcmp(argv[1], "--help"));

   fd = open(argv[1], O_RDONLY);
   assert (fd != -1);

   totRequired = 0;

   iov[0].iov_base = &myStruct;
   totRequired += sizeof(struct stat);

   iov[1].iov_base = &x;
   totRequired += sizeof(x);

   iov[2].iov_base = str;
   totRequired += STR_SIZE;

   numRead = readv(fd, iov, 3);
   assert (numRead != -1);

   if (numRead < totRequired)
      printf("Read fewer bytes than requested\n");

   printf("total bytes requested: %lf; bytes read: %ld\n", 
          (long) totRequired, (long) numRead);

   exit(EXIT_SUCCESS); 
}
