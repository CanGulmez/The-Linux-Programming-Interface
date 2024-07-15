/* The program uses 'readlink()' and 'realpath()' to read the contents 
   of a symbolic link and to resolve the link to an absolute pathname. 
*/

#include "./utils.h"

#define BUF_SIZE PATH_MAX

void main(int argc, char *argv[])
{  
   if (argc != 2 || !strcmp(argv[1], "--help"))
      raiseError("wrong command-line arguments", file, line);

   struct stat statbuf;
   char buf[BUF_SIZE];
   ssize_t numBytes;

   if (lstat(argv[1], &statbuf) == -1)
      raiseError("lstat() did not work", file, line);

   if (!S_ISLNK(statbuf.st_mode))
      raiseError("S_ISLNK() did not work", file, line);

   numBytes = readlink(argv[1], buf, BUF_SIZE - 1);
   if (numBytes == -1)
      raiseError("readlink() did not work", file, line);

   buf[numBytes] = '\0';         /* Add terminating null byte */
   printf("readlink: %s --> %s\n", argv[1], buf);

   if (realpath(argv[1], buf) == NULL)
      raiseError("realpath() did not work", file, line);

   printf("realpath: %s --> %s\n", argv[1], buf);

   exit(EXIT_SUCCESS);
}
