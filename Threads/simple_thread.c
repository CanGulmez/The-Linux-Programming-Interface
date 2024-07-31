/* A simple program using Pthreads */

#include "./utils.h"

void *threadFunc(void *arg)
{
   printf("%s", (char *) arg);

   return (void *) strlen(arg);
}

void main(int argc, char *argv[])
{
   pthread_t t1;
   void *res;
   int s;

   s = pthread_create(&t1, NULL, threadFunc, (void *) "Hello world\n");
   if (s != 0)
      destroyProgram("pthread_create()"); 
 
   printf("Mesage from main()\n");

   s = pthread_join(t1, &res);
   if (s != 0)
      destroyProgram("pthread_join()");

   printf("Thread returned %ld\n", (long) res);
 
   exit(EXIT_SUCCESS);
}
