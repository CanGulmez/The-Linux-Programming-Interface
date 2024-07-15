/* Memory Allocation */

#include "./free_and_sbrk.h"

#define MAX_ALLOCS   1000000

void main(int argc, char *argv[])
{
   char *buffer[MAX_ALLOCS];
   int freeStep, freeMin, freeMax, blockSize, numAllocs, i;

   printf("\n");

   if (argc < 3 || !strcmp(argv[1], "--help"))
      raiseError("wrong command-line arguments");

   numAllocs = atoi(argv[1]);
   if (numAllocs > MAX_ALLOCS)
      raiseError("exceeded maximum allocation");

   blockSize = atoi(argv[2]);

   freeStep = (argc > 3) ? atoi(argv[3]) : 1;
   freeMin = (argc > 4) ? atoi(argv[4]) : 1;
   freeMax = (argc > 5) ? atoi(argv[5]) : numAllocs;

   if (freeMax > numAllocs)
      raiseError("exceeded maximum allocation");

   printf("Initial program break:      %10p\n", sbrk(0));
   printf("Allocating %d*%d bytes\n", numAllocs, blockSize);

   for (i=0; i<numAllocs; i++)
   {
      buffer[i] = malloc(blockSize);
      if (buffer[i] == NULL)
         raiseError("malloc() retuned NULL");
   }

   printf("Program break is now:       %10p\n", sbrk(0));

   printf("Freeing blocks from %d to %d in steps of %d\n", 
          freeMin, freeMax, freeStep);
   
   for (i=freeMax - 1; i<freeMax; i+=freeStep)
      free(buffer[i]);

   printf("After free(), program break is: %10p\n", sbrk(0));

   exit(EXIT_SUCCESS);
}
