/* Copying data into a POSIX shared memory object */

#include "./utils.h"

void main(int argc, char *argv[])
{
   int fd;
   size_t len;       /* Size of shared memory object */
   void *addr;

   if (argc != 3 || !strcmp(argv[1], "--help"))
      raiseError("wrong-command line aeguments");

   fd = shm_open(argv[1], O_RDWR, 0);     /* Open existing object */
   if (fd == -1)
      raiseError("shm_open()");

   len = strlen(argv[2]);
   if (ftruncate(fd, len) == -1)          /* Resize object to hold string */
      raiseError("ftruncate()");

   printf("Resized to %ld bytes\n", (long) len);

   addr = mmap(NULL, len, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
   if (addr == NULL)
      raiseError("mmap()");

   if (close(fd) == -1)                   /* 'fd' is no longer needed */
      raiseError("close()");

   printf("copying %ld bytes\n", (long) len);
   memcpy(addr, argv[2], len);            /* Copy string to shared memory */

   exit(EXIT_SUCCESS);
}
