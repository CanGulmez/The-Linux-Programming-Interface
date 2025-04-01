/* Using 'mmap()' to create a shared file mapping */

#include "../linux.h"

#define MEM_SIZE 10

void main(int argc, char *argv[])
{
   char *addr;
   int fd;

   if (argc < 2 || strcmp(argv[1], "--help") == 0)
      usage_error("Wrong command-line usage");

   fd = open(argv[1], O_RDWR);
   if (fd == -1)
      syscall_error();

   addr = mmap(NULL, MEM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
   if (addr == MAP_FAILED)
      syscall_error();

   if (close(fd) == -1)       /* No longer need 'fd' */
      syscall_error();

   printf("Current string=%.*s\n", MEM_SIZE, addr);

   if (argc > 2) {            /* Update contents of region */
      if (strlen(argv[2]) >= MEM_SIZE)
         usage_error("String too long");
      
      memset(addr, 0, MEM_SIZE); /* Zero out region */
      strncpy(addr, argv[2], MEM_SIZE - 1);
      if (msync(addr, MEM_SIZE, MS_SYNC) == -1)
         syscall_error();

      printf("Copied \"%s\" to shared memory\n", argv[2]);
   }

   exit(EXIT_SUCCESS);
}
