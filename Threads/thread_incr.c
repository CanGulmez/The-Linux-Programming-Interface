/* Oncorrectly incrementing a global variable from two threads */

#include "./utils.h"

static int glob = 0;

/* Loop 'arg' times incrementing 'glob' */

void *threadFunc(void *arg)
{
   int loop = *((int *) arg);
   int loc, i; 

   for (i=0; i<loop; i++)
   {
      loc = glob; 
      loc ++;
      glob = loc;
   }
   return NULL;
}

void main(int argc, char *argv[])
{
   pthread_t t1, t2;
   int loops, s;

   loops = (argc > 1) ? atoi(argv[1]) : 100; 

   s = pthread_create(&t1, NULL, threadFunc, (void *) &loops);
   if (s != 0)
      destroyProgram("pthread_create()");
   
   s = pthread_create(&t2, NULL, threadFunc, (void *) &loops);
   if (s != 0)
      destroyProgram("pthread_create()");

   s = pthread_join(t1, NULL);
   if (s != 0)
      destroyProgram("pthread_join()");
   
   s = pthread_join(t2, NULL);
   if (s != 0)
      destroyProgram("pthread_join()");

   printf("glob = %d\n", glob);

   exit(EXIT_SUCCESS);
}
