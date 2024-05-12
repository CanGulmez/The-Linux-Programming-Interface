/* The program can be used to illustrate the effect of 'free()' 
   on the program break. This program allocates multiple blocks 
   of memory and then frees some or all of them, depending on
   its (optional) command-line arguments.

   The first two command-line arguments specify the number and
   size of blocks to allocate. The third command-line agument
   specifies the loop step unit to be used when freeing memory 
   blocks. If we specify 1 here (which is also the default if 
   this argument is omitted), then the program frees every 
   memory block; if 2, then every second allocated block; and 
   so on. The fourth and fifth command-line arguments specify
   the range of blocks that we wish to free. If these arguments
   are omitted, then all allocated blocks (in steps given by the
   third command-line argument) are freed.
*/

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>

#define MAX_ALLOCS 1000000

void main(int argc, char *argv[])
{
   char *ptr[MAX_ALLOCS];
   int freeStep, freeMin, freeMax, blockSize, numAllocs;

   printf("\n");

   assert (argc >= 3 && strcmp(argv[1], "--help")); 

   numAllocs = atoi(argv[1]);
   assert (numAllocs <= MAX_ALLOCS);

   blockSize = atoi(argv[2]);

   freeStep = (argc > 3) ? atoi(argv[3]) : 1;
   freeMin = (argc > 4) ? atoi(argv[4]) : 1;
   freeMax = (argc > 5) ? atoi(argv[5]) : numAllocs; 

   assert (freeMax <= numAllocs);


   printf("Initial program break:   %10p\n", sbrk(0));
   printf("Allocating %d*%d bytes\n", numAllocs, blockSize);

   for (int i=0; i<numAllocs; i++)
   {
      ptr[i] = malloc(blockSize);
      assert (ptr[i] != NULL);
   }
   printf("Program break is now:    %10p\n", sbrk(0));

   printf("Freeing blocks from %d to %d in steps of %d\n", 
          freeMin, freeMax, freeStep);
   
   for (int i=freeMin - 1; i<freeMax; i += freeStep)
      free(ptr[i]);

   printf("After free() program break is:    %10p\n", sbrk(0));

   exit(EXIT_SUCCESS);
}
