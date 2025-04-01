/* Performing scatter input with 'readv()' */

#include "../linux.h"

void main(int argc, char *argv[])
{
   int fd;
   struct iovec iov[3];
   struct stat myStruct;
   int x;
#define  STR_SIZE 100
   char str[STR_SIZE];
   ssize_t numRead, totRequested;

   if (argc != 2 || strcmp(argv[1], "--help") == 0)
      usage_error("Wrong command line usage");

   fd = open(argv[1], O_RDONLY);
   if (fd == -1)
      syscall_error();

   totRequested = 0;

   iov[0].iov_base = &myStruct;
   iov[0].iov_len = sizeof(struct stat);
   totRequested += iov[0].iov_len;

   iov[1].iov_base = &x;
   iov[1].iov_len = sizeof(x);
   totRequested += iov[1].iov_len;

   iov[2].iov_base = str;
   iov[2].iov_len = STR_SIZE;
   totRequested += iov[2].iov_len;

   numRead = readv(fd, iov, 3);
   if (numRead == -1)
      syscall_error();

   if (numRead < totRequested)
      printf("Read fewer bytes than requested\n");

   printf("total bytes requested: %ld; bytes read: %ld\n", 
      (long) totRequested, (long) numRead);

   exit(EXIT_SUCCESS);
}
