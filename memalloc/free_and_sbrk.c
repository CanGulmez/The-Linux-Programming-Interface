/* Demonstrate what happends to the program break when memory is freed */

#include "../linux.h"

#define MAX_ALLOCS      1000000

void main(int argc, char *argv[])
{
   char *ptr[MAX_ALLOCS];
   int freeStep, freeMin, freeMax, blockSize, numAllocs, i;

   if (argc < 3 || strcmp(argv[1], "--help") == 0)
      usage_error("Wrong command-line usage");

   numAllocs = atoi(argv[1]);
   if (numAllocs > MAX_ALLOCS)
      usage_error("Exceed permitable allocation number");

   blockSize = atoi(argv[2]);

   freeStep = (argc > 3) ? atoi(argv[3]) : 1;
   freeMin = (argc > 4) ? atoi(argv[4]) : 1;
   freeMax = (argc > 5) ? atoi(argv[5]) : numAllocs;

   if (freeMax > numAllocs)
      usage_error("Exceed permitable allocation number");

   printf("Initial program break: %10p\n", sbrk(0));
   printf("Allocating %d*%d bytes\n", numAllocs, blockSize);

   for (i = 0; i < numAllocs; i++) {
      ptr[i] = malloc(blockSize);
      if (ptr[i] == NULL)
         break;
   }
   printf("Program break is now: %10p\n", sbrk(0));

   printf("Freeing blocks from %d to %d in steps of %d\n",
      freeMin, freeMax, freeStep);
   for (i = freeMin - 1; i < freeMax; i++)
      free(ptr[i]);

   printf("After free(), program break is: %10p\n", sbrk(0));

   exit(EXIT_SUCCESS);
}
