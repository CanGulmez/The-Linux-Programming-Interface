/* Using 'mlock()' and 'mincore()' */

#include "../linux.h"

/* Display residency of pages in range [addr .. (addr + length - 1)] */

static void displayMincore(char *addr, size_t length)
{
   unsigned char *vec;
   long pageSize, numPages, i;

   pageSize = sysconf(_SC_PAGESIZE);
   numPages = (length + pageSize - 1) / pageSize;
   vec = malloc(numPages);
   if (vec == NULL)
      syscall_error();

   if (mincore(addr, length, vec) == -1)
      syscall_error();

   for (i = 0; i < numPages; i++) {
      if (i % 64 == 0)
         printf("%s%10p: ", (i == 0) ? "" : "\n", addr + (i * pageSize));
      printf("%c", (vec[i] & 1) ? '*' : '.');
   }
   printf("\n");

   free(vec);
}  

void main(int argc, char *argv[])
{
   char *addr;
   size_t len, lockLen;
   long pageSize, stepSize, i;

   if (argc != 4 || strcmp(argv[1], "--help") == 0)
      usage_error("Wrong command-line usage");

   pageSize = sysconf(_SC_PAGESIZE);
   if (pageSize == -1)
      syscall_error();

   len = atoi(argv[1]);
   stepSize = atoi(argv[2]);
   lockLen = atoi(argv[3]);

   addr = mmap(NULL, len, PROT_READ, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
   if (addr == MAP_FAILED)
      syscall_error();

   printf("Allocated %ld (%#lx) bytes starting at %p\n",
      (long) len, (unsigned long) len, addr);

   printf("Before mlock:\n");
   displayMincore(addr, len);

   /* Lock pages specified by command line arguments into memory */

   for (i = 0; i + lockLen <= len; i += stepSize)
      if (mlock(addr + i, lockLen) == -1)
         syscall_error();
   
   printf("After mlock:\n");
   displayMincore(addr, len);

   exit(EXIT_SUCCESS);
}
