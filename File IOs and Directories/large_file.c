/* The program opens the specified file, seeks to the given file offset, 
   and then writes a string. 
*/

#include "./utils.h"

// #define _FILE_OFFSET_BITS  64

void main(int argc, char *argv[])
{
   int fd;
   off64_t off;

   if (argc != 3 || !strcmp(argv[1], "--help"))
      raiseError("wrong command-line arguments", file, line);

   fd = open64(argv[1], O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
   if (fd == -1)
      raiseError("open64() did not work", file, line);

   off = atoll(argv[2]);
   if (lseek64(fd, off, SEEK_SET) == -1)
      raiseError("lseek64() did not work", file, line);

   if (write(fd, "test", 4) == -1)
      raiseError("write() did not work", file, line);

   exit(EXIT_SUCCESS);
}
