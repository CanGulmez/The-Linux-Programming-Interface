/* Changing memory protection with 'mprotect()' */

#include "./utils.h"

#define LEN          (1024 * 1024)
#define SHELL_FMT    "cat /proc/%ld/maps | grep zero"
#define CMD_SIZE     (sizeof(SHELL_FMT) + 20)   
                     /* Allow extra space for integer string */

void main(int argc, char *argv[])
{
   char command[CMD_SIZE];
   char *addr;

   /* Create an anonymous mapping with all acces denied */

   addr = mmap(NULL, LEN, PROT_NONE, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
   if (addr == MAP_FAILED)
      raiseError("mmap()"); 

   /* Display line from /proc/self/maps corresponding to mapping */

   printf("Before mprotect()\n");
   snprintf(command, CMD_SIZE, SHELL_FMT, (long) getpid()); 
   system(command);

   /* Change protection on memory to allow read and write access */

   if (mprotect(addr, LEN, PROT_READ | PROT_WRITE) == -1)
      raiseError("mprotect()");

   printf("After mprotect()\n");
   system(command);        /* Review protection via /proc/self/maps */

   exit(EXIT_SUCCESS);
}
