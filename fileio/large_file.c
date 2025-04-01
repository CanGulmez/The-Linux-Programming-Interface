/* Accessing large files */

#include "../linux.h"

void main(int argc, char *argv[])
{
   int fd;
   off64_t off;

   if (argc != 3 || strcmp(argv[1], "--help") == 0)
      usage_error("Wrong command-line usage");

   fd = open64(argv[1], O_RDWR | O_CREAT, S_IRUSR | S_IWUSR);
   if (fd == -1)
      syscall_error();

   off = atoll(argv[2]);
   if (lseek64(fd, off, SEEK_SET) == -1)
      syscall_error();

   if (write(fd, "test", 4) == -1)
      syscall_error();

   exit(EXIT_SUCCESS);
}
