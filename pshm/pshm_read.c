/* Copying data from a POSIX shared memory object */

#include "../linux.h"

void main(int argc, char *argv[])
{
   int fd;
   char *addr;
   struct stat sb;

   if (argc != 2 || strcmp(argv[1], "--help") == 0)
      usage_error("Wrong command-line usage");

   fd = shm_open(argv[1], O_RDONLY, 0);
   if (fd == -1)
      syscall_error();

   /* Use shared memory object size as length argument for mmap()
      and as number of bytes to write() */

   if (fstat(fd, &sb) == -1)
      syscall_error();

   addr = mmap(NULL, sb.st_size, PROT_READ, MAP_SHARED, fd, 0);
   if (addr == MAP_FAILED)
      syscall_error();

   if (close(fd) == -1)
      syscall_error();

   write(STDOUT_FILENO, addr, sb.st_size);
   printf("\n");

   exit(EXIT_SUCCESS);
}
