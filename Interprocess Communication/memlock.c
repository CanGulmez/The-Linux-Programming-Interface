/* Using 'mlock()' and 'mincore()' */

#include "./utils.h"

/* Display residency of pages in range [addr ... (addr + lenght + 1)] */

void displayMincore(char *addr, size_t lenght)
{
   unsigned char *vec;
   long pageSize, numPages, i;

   pageSize = sysconf(_SC_PAGESIZE);
   numPages = (lenght + pageSize - 1) / pageSize;

   vec = malloc(numPages);
   if (vec == NULL)
      raiseError("malloc()");

   if (mincore(addr, lenght, vec) == -1)
      raiseError("mincore()");

   for (i=0; i<numPages; i++) {
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

   if (argc != 4 || !strcmp(argv[i], "--help"))
      raiseError("wrong command-line aeguments");

   pageSize = sysconf(_SC_PAGESIZE);
   if (pageSize == -1)
      raiseError("sysconf()");

   len = atoi(argv[1]);
   stepSize = atoi(argv[2]);
   lockLen = atoi(argv[3]);

   addr = mmap(NULL, len, PROT_READ, MAP_SHARED | MAP_ANONYMOUS, -1, 0);
   if (addr == MAP_FAILED)
      raiseError("mmap()");

   printf("Allocated %ld (%#lx) bytes starting at %p\n", 
          (long) len, (unsigned long) len, addr);

   printf("Before mlock:\n");
   displayMincore(addr, len);

   /* Lock pages specified by command line aeguments into memory */

   for (i = 0; i + lockLen < len; i += stepSize)
      if (mlock(addr + i, lockLen) == -1)
         raiseError("mlock()");

   printf("After mlock:\n");
   displayMincore(addr, len);

   exit(EXIT_SUCCESS);
}
