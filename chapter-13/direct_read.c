/* The program provides a simple example of the use of O_DIRECT
   while opening a file for reading. This program takes up to four
   command-line arguments specifying, in order, the file to be read,
   the number of bytes to be read from the file. the offset to which
   the program should seek before reading from the file and the
   alignment of the data buffer passed to 'read()'. The last two
   arguments are optional, and default to offset 0 and 4096 bytes, 
   respectively. */

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <fcntl.h>
#include <malloc.h>

void main(int argc, char *argv[])
{
   int fd;
   ssize_t numRead;
   ssize_t lenght, alignment;
   off_t offset;
   void *buf; 

   assert (argc >= 3 && strcmp(argv[1], "--help"));

   lenght = atol(argv[2]);
   offset = (argc > 3) ? atol(argv[3]) : 0;
   alignment = (argc > 4) ? atol(argv[4]) : 4096;

   fd = open(argv[1], O_RDONLY | O_DIRECT);
   assert (fd != -1);

   // memalign() allocates a block of memory aligned on an address
   // that is a multiple of its first argument. The following 
   // expression ensures that 'buf' is aligned on a non-power-of-two
   // multiple of 'alignment'. We do this to ensure that if, for
   // example, we ask for a 256-byte aligned buffer, then we don't
   // accidentally get a buffer that is also aligned on a 512-byte
   // boundary.

   // The '(char *)' cast is needed to allow pointer arithmetic 
   // (which is not possible on the 'void *' returned by memalign()).

   buf = (char *) memalign(alignment * 2, lenght + alignment) + 
                                                   alignment;
   assert (buf != NULL);

   assert (lseek(fd, offset, SEEK_SET) != -1);

   numRead = read(fd, buf, lenght);
   assert (numRead != -1);

   printf("Read %ld bytes\n", (long) numRead);

   exit(EXIT_SUCCESS);
}
