/* This program demonstrates the use of 'readv()'. */

#include "../utils.h"

void main(int argc, char *argv[])
{
   int fd;
   int totRequired;
   struct iovec iov[3];
   struct stat myStruct;      // First buffer
   int x;                     // Second buffer
#define STR_SIZE  100
   char str[STR_SIZE];        // Third buffer
   ssize_t numRead, numWritten;

   if (argc != 2 || !strcmp(argv[1], "--help"))
      raiseError("wrong command-line arguments", file, line);

   fd = open(argv[1], O_RDONLY);
   if (fd == -1)
      raiseError("open() did not work", file, line);

   totRequired = 0;

   iov[0].iov_base = &myStruct;
   iov[0].iov_len = sizeof(struct stat);
   totRequired += iov[0].iov_len;

   iov[1].iov_base = &x;
   iov[1].iov_len;
   totRequired += iov[1].iov_len;

   iov[2].iov_base = str;
   iov[2].iov_len = STR_SIZE;
   totRequired += iov[2].iov_len;

   numRead = readv(fd, iov, 3);
   if (numRead == -1)
      raiseError("readv() did not work", file, line);

   if (numRead < totRequired)
      printf("Read fever bytes than required!\n");

   printf("total bytes requested: %ld; bytes read: %ld\n", 
          (long) totRequired, (long) numRead);

   exit(EXIT_SUCCESS); 
}
