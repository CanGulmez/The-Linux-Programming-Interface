/* Using a mutex to protect access to a global variable */

#include "../linux.h"

static int glob = 0;
static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;

static void *threadFunc(void *arg)
{
   int loops = *((int *) arg);
   int loc, i, s;

   for (i = 0; i < loops; i++) {
      s = pthread_mutex_lock(&mtx);
      if (s != 0)
         syscall_error();
      
      loc = glob;
      loc ++;
      glob = loc;

      s = pthread_mutex_unlock(&mtx);
      if (s != 0)
         syscall_error();
   }

   return NULL;
}

void main(int argc, char *argv[])
{
   pthread_t thread1, thread2;
   int loops, s;

   loops = (argc > 1) ? atoi(argv[1]) : 10000000;

   s = pthread_create(&thread1, NULL, threadFunc, &loops);
   if (s != 0)
      syscall_error();
   s = pthread_create(&thread2, NULL, threadFunc, &loops);
   if (s != 0)
      syscall_error();

   s = pthread_join(thread1, NULL);
   if (s != 0)
      syscall_error();
   s = pthread_join(thread2, NULL);
   if (s != 0)
      syscall_error();

   printf("glob = %d\n", glob);
   exit(EXIT_SUCCESS);
}
