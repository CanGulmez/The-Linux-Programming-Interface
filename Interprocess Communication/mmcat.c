/* Using 'mmap()' to create a private file mapping */

#include "./utils.h"

void main(int argc, char *argv[])
{
   int *addr;
   int fd;
   struct stat sb;

   if (argc != 2 || !strcmp(argv[1], "--help"))
      raiseError("wrong command-line argument");

   fd = open(argv[1], O_RDONLY);
   if (fd == -1)
      raiseError("open()");
 
   /* Obtain the size of the file and use it to specify the size of 
      the mapping and the size of the buffer to be written */

   if (fstat(fd, &sb) == -1)
      raiseError("fstat()");

   addr = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
   if (addr == MAP_FAILED)
      raiseError("mmap()");

   printf("0x%x\n", addr);

   exit(EXIT_SUCCESS); 
}
