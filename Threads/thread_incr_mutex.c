/* Using a mutex to proctect access to a global variable */

#include "./utils.h"

static int glob = 0;
static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

/* Loop 'arg' times incrementing 'glob' */
static void *threadFunc(void *arg)
{
   int loops = *((int *) arg);
   int loc, i, s;

   for (i=0; i<loops; i++)
   {
      s = pthread_mutex_lock(&mtx); 
      if (s != 0)
         destroyProgram("pthread_mutex_lock()");

      glob ++;

      s = pthread_mutex_unlock(&mtx);
      if (s != 0)
         destroyProgram("pthread_mutex_unlock()");
   }
   return;
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
