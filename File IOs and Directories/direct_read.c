/* The program a simple example of the use O_DIRECT while opening a file for 
   reading. This progrsm takes up to four command-line arguments specifying, 
   in order, the file to be read, the number of bytes to be read from bytes.
   the offset to which the program sould seek before reading from the file, 
   and the allignment of the data buffer passed to 'read()'. The last 
   arguments are optional, and default to offset 0 and 4096 bytes. 
*/

#include "../utils.h"

void main(int argc, char *argv[])
{
   int fd;
   ssize_t numRead;
   size_t lenght, alignment;
   off_t offset;
   void *buf;

   if (argc < 3 || !strcmp(argv[1], "--help"))
      raiseError("wrong command-line arguments", file, line);

   lenght = atol(argv[2]);
   offset = (argc > 3) ? atol(argv[3]) : 0;
   alignment = (argc > 4) ? atol(argv[4]) : 4096;

   fd = open(argv[1], O_RDONLY | O_DIRECT);
   if (fd == -1)
      raiseError("open() did not work", file, line);

   /* memalign() allocates a block of memory aligned on an address that
      is a multiple of its first argument. The fllowing expression
      ensures that 'buf' is aligned on a non-power-of-two multiple of
      'alignment'. We do thid to ensure that if, for example, we ask
      for a 256-byte aligned buffer, then we don't accidentally get
      a buffer that is also aligned on a 512-byte boundary. 
   
      The '(char *)' cast is needed to allow pointer arithmetic (which
      is not possible on the 'void *' returned by memalign()).
   */

   buf = (char *) memalign(alignment * 2, lenght + alignment) + alignment;
   if (buf == NULL)
      raiseError("memalign() did not work", file, line);

   if (lseek(fd, offset, SEEK_SET) == -1)
      raiseError("lseek() did not work", file, line);

   numRead = read(fd, buf, lenght);
   if (numRead == -1)
      raiseError("read() did not work", file, line);

   printf("Read %ld bytes\n", (long) numRead);

   exit(EXIT_SUCCESS);
}
