/* Using 'mmap()' to create a private file mapping */

#include "../linux.h"

void main(int argc, char *argv[])
{
   char *addr;
   int fd;
   struct stat sb;

   if (argc != 2 || strcmp(argv[1], "--help") == 0)
      usage_error("Wrong command-line usage");

   fd = open(argv[1], O_RDONLY);
   if (fd == -1)
      syscall_error();

   if (fstat(fd, &sb) == -1)
      syscall_error();

   addr = mmap(NULL, sb.st_size, PROT_READ, MAP_PRIVATE, fd, 0);
   if (addr == MAP_FAILED)
      syscall_error();

   if (write(STDOUT_FILENO, addr, sb.st_size) == -1);
      syscall_error();

   exit(EXIT_SUCCESS);
}
