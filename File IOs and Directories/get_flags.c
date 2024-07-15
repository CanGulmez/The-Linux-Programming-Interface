/* This program is replated with getting and setting flag status related
   to opening file. 
*/

#include "./utils.h"

#define BUF_SIZE     1024

void main(int argc, char *argv[])
{
   int fd, flags, accessMode;
   ssize_t numRead, numWritten;
   char buffer[BUF_SIZE];

   /* Open the existing file  */

   fd = open("./newfile.txt", O_RDWR);
   if (fd == -1)
      raiseError("open() did not work", file, line);

   flags = fcntl(fd, F_GETFL);
   if (flags == -1)
      raiseError("fcntl() did not work", file, line);

   /* Get the open file flags */

   accessMode = flags & O_ACCMODE;

   if (accessMode == O_RDONLY)
      printf("file is just readable\n"); 
   else if (accessMode == O_WRONLY)
      printf("file is just writible\n");
   else if (accessMode == O_RDWR)
      printf("file is readable and writible\n");
   else
      raiseError("access mode did not retrieved", file, line);

   exit(EXIT_SUCCESS); 
}

