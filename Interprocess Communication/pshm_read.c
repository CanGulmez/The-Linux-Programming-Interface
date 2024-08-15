/* Copying data from a POSIX shared memory object */

#include "./utils.h"

void main(int argc, char *argv[])
{
   int fd;
   void *addr;
   struct stat sb;

   if (argc != 2 || !strcmp(argv[1], "--help"))
      raiseError("wrong-command line aeguments");

   fd = shm_open(argv[1], O_RDWR, 0);     /* Open existing object */
   if (fd == -1)
      raiseError("shm_open()");

   /* Use shared memory object size as lenght argument for mmap()
      and as number of bytes to write() */

   if (fstat(fd, &sb) == -1)
      raiseError("fstat()");

   addr = mmap(NULL, sb.st_size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
   if (addr == NULL)
      raiseError("mmap()");

   if (close(fd) == -1)                   /* 'fd' is no longer needed */
      raiseError("close()");

   write(STDOUT_FILENO, addr, sb.st_size);
   printf("\n");
   exit(EXIT_SUCCESS);
}
