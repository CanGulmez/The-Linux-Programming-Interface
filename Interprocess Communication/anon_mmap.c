/* Sharing an anonymous mapping between parent and child processes */

#include "./utils.h"

void main(int argc, char *argv[])
{
   int *addr;        /* Pointer to shared memory region */

   addr = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, 
               MAP_SHARED | MAP_ANONYMOUS, -1, 0);
   if (addr == MAP_FAILED)
      raiseError("mmap()");

   /* or... */

   // int fd;           /* Map /dev/zero */

   // fd = open("/dev/zero", O_RDWR);
   // if (fd == -1)
   //    raiseError("open()");

   // addr = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
   // if (addr == MAP_FAILED)
   //    raiseError("mmap()");

   // if (close(fd) == -1)       /* No longer neede */
   //    raiseError("close()");

   *addr = 1;           /* Initialize integer in mapped region */

   switch (fork()) {    /* Parent and child share mapping */

      case -1:
         raiseError("fork()");

      case 0:           /* Child: increment shared integer and exit */
         printf("Child started, value = %d\n", *addr);
         (*addr) ++;

         if (munmap(addr, sizeof(int)) == -1)
            raiseError("munmap()");

         exit(EXIT_SUCCESS);

      default:           /* Parent: wait for child to terminate */
         if (wait(NULL) == -1)
            raiseError("wait()");

         printf("In parent, value = %d\n", *addr); 

         if (munmap(addr, sizeof(int)) == -1)
            raiseError("munmap()");

         exit(EXIT_SUCCESS);
   }   
}
