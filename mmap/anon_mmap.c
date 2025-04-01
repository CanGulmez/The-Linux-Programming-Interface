/* Sharing an anonymous mapping between parent and child processes */

#include "../linux.h"

void main(int argc, char *argv[])
{
   int *addr, fd;

   addr = mmap(NULL, sizeof(int), PROT_READ | PROT_WRITE, 
               MAP_SHARED | MAP_ANONYMOUS, -1, 0);
   if (addr == MAP_FAILED)
      syscall_error();

   *addr = 1;           /* Initialize integer in mapped region */

   switch (fork()) {    /* Parent and child share mapping */
      case -1:
         syscall_error();

      case 0:            /* Child: increment shared integer and exit */
         printf("Child started, value = %d\n", *addr);
         (*addr)++;

         if (munmap(addr, sizeof(int)) == -1)
            syscall_error();
         exit(EXIT_SUCCESS);

      default:           /* Parent: wait for child to exit */
         if (wait(NULL) == -1)
            syscall_error();
         printf("In parent, value = %d\n", *addr);
         
         if (munmap(addr, sizeof(int)) == -1)
            syscall_error();
         exit(EXIT_SUCCESS);
   }
}
