/* Using 'mmap()' to create a shared file mapping */

#include "./utils.h"

#define MEM_SIZE     10

void main(int argc, char *argv[])
{
   char *addr;
   int fd;

   if (argc < 2 || !strcmp(argv[1], "--help"))
      raiseError("wrong command-line arguments"); 

   fd = open(argv[1], O_RDWR);
   if (fd == -1)
      raiseError("open()");

   addr = mmap(NULL, MEM_SIZE, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
   if (addr == MAP_FAILED)
      raiseError("mmap()");

   if (close(fd) == -1)       /* No longer need 'fd' */
      raiseError("close()");

   if (argc > 2) {            /* Update contents of region */
      if (strlen(argv[2]) >= MEM_SIZE)
         raiseError("strlen()");

      memset(addr, 0, MEM_SIZE); /* Zero out region */
      strncpy(addr, argv[2], MEM_SIZE - 1);
      if (msync(addr, MEM_SIZE, MS_SYNC) == -1)
         raiseError("msync()");

      printf("Copied \"%s\" to shared memory\n", argv[2]);
   }

   exit(EXIT_SUCCESS);
}