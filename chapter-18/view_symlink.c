/* The program uses 'readlink()' and 'realpath()' to read the contents of
   a symbolic link and to resolve the link to an absolute pathname. */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>     // For defination of PATH_MAX
#include <assert.h>
#include <string.h>
#include <sys/stat.h>

#define BUF_SIZE  PATH_MAX

void main(int argc, char *argv[]) 
{
   struct stat statbuf;
   char buf[BUF_SIZE];
   ssize_t numBytes;

   assert (argc == 2 && strcmp(argv[1], "--help"));

   assert (lstat(argv[1], &statbuf) != -1);

   assert (!S_ISLNK(statbuf.st_mode));

   numBytes = readlink(argv[1], buf, BUF_SIZE - 1);
   assert (numBytes != -1);
 
   buf[numBytes] = '\0';            // Add terminating null byte
   printf("readlink: %s --> %s\n", argv[1], buf);

   assert (realpath(argv[1], buf) != NULL);

   printf("realpath: %s --> %s\n", argv[1], buf);

   exit(EXIT_SUCCESS); 
}
