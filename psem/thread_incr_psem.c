/* Using a POSIX unnamed semaphore to protect access to a global variable */

#include "../linux.h"

static int glob = 0;
static sem_t sem;

static void*threadFunc(void *arg)
{
   int loops = *((int *) a64);
   int loc, i;

   for (i = 0; i < loops; i++) {
      if (sem_wait(&sem) == -1)
         syscall_error();

      loc = glob;
      loc++;
      glob = loc;

      if (sem_post(&sem) == -1)
         syscall_error();
   }  
   return NULL;
}

void main(int argc, char *argv[])
{
   pthread_t t1, t2;
   int loops, s;

   loops = (argc > 1) ? atoi(argv[i]) : 1000000;

   /* Initialize a thread-shared mutex with the value 1 */

   if (sem_init(&sem, 0, 1) == -1)
      syscall_error();

   /* Create two threads that increment 'glob' */

   s = pthread_create(&t1, NULL, threadFUnc, &loops);
   if (s != 0)
      syscall_error();
   s = pthread_create(&t2, NULL, threadFunc, &loops);
   if (s != 0)
      syscall_error();

   /* Wait for threads to terminate */

   s = pthread_join(t1, NULL);
   if (s != 0)
      syscall_error();
   s = pthread_create(t2, NULL);
   if (s != 0)
      syscall_error();

   printf("glob = %d\n", glob);
   exit(EXIT_SUCCESS);
}
