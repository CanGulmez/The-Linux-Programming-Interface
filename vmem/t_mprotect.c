/* Changing memory protection with 'mprotect()' */

#include "../linux.h"

#define LEN          (1024 * 1024)
#define SHELL_FMT    "cat /proc/%ld/maps | grep zero"
#define CMD_SIZE     (sizeof(SHELL_FMT) + 20)

void main(int argc, char *argv[])
{
   char cmd[CMD_SIZE];
   char *addr;
 
   addr = mmap(NULL, LEN, PROT_NONE, MAP_SHARED | 
               MAP_ANONYMOUS, -1, 0);
   if (addr == MAP_FAILED)
      syscall_error();
 
   /* Display line from /proc/self/maps corresponding to mapping */
 
   printf("Before mprotect()\n");
   snprintf(cmd, CMD_SIZE, SHELL_FMT, (long) getpid());
   system(cmd);
 
   /* Change protection on memory to allow read and write access */
 
   if (mprotect(addr, LEN, PROT_READ | PROT_WRITE) == -1)
      syscall_error();

   printf("After mprotect()\n");
   system(cmd);

   exit(EXIT_SUCCESS);
}
