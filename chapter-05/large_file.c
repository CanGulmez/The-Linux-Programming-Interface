/* This program takes two command-line arguments: the name of a 
   file to be opened and an integer valuespecifying a file offset.
   The program opens the specified file, seeks to the given file
   offset, and then writes a string. The following shell session
   demonstrates the use of the program to seek to a very large offset in the file (greater than 10GB) and then write some 
   bytes. */

#define _LARGEFILE64_SOURCE

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>

void main(int argc, char *argv[])
{
   int fd;
   off64_t off; 

   assert (argc == 3 && strcmp(argv[1], "--help"));

   fd = open64(argv[1], O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
   assert (fd != -1);

   off = atoll(argv[2]);

   assert (lseek64(fd, off, SEEK_SET) != -1);

   assert (write(fd, "test", 4) != -1);

   exit(EXIT_SUCCESS);
}
