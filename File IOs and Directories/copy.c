/* This program is simple version of the 'c1p(1)' command. It copies the
   contents of the existing filenamed in its first command-line argument
   to the new file named in its second command-line argument. 
*/

#include "./utils.h"

#ifndef BUF_SIZE
#define BUF_SIZE     1024
#endif

void main(int argc, char *argv[])
{
   int inputFd, outputFd, openFlags;
   mode_t filePerms;
   ssize_t numRead; 
   char buf[BUF_SIZE];

   if (argc != 3 || !strcmp(argv[1], "--help"))
      raiseError("wrong command-line arguments", file, line);

   /* Open input and output files */

   inputFd = open(argv[1], O_RDONLY);
   if (inputFd == -1)
      raiseError("open() did not work", file, line);

   openFlags = O_CREAT | O_WRONLY | O_TRUNC;
   filePerms = S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP |
               S_IROTH | S_IWOTH;   /* rw-rw-rw */

   outputFd = open(argv[2], openFlags, filePerms);
   if (outputFd == -1)
      raiseError("open() did not work", file, line);

   /* Transfer data until we encounter end of input or an error */

   while ((numRead = read(inputFd, buf, BUF_SIZE)) > 0)
      if (write(outputFd, buf, numRead) != numRead)
         raiseError("write() did not work!", file, line);

   if (numRead == -1)
      raiseError("read() did not work", file, line);

   if (close(inputFd) == -1)
      raiseError("close() did not work", file, line);
   if (close(outputFd) == -1)
      raiseError("close() did not work", file, line);

   exit(EXIT_SUCCESS);
}
