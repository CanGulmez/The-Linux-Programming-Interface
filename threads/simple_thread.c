/* A simple program using Pthreads */

#include "../linux.h"

static void *threadFunc(void *arg)
{
   char *s = (char *) arg;

   printf("%s", s);

   return (void *) strlen(s);
}

void main(int argc, char *argv[])
{
   pthread_t thread;
   void *res;
   int s;

   s = pthread_create(&thread, NULL, threadFunc, "Hello world\n");
   if (s != 0)
      syscall_error();

   printf("Message from main()\n");
   s = pthread_join(thread, &res);
   if (s != 0)
      syscall_error();

   printf("Thread returned %ld\n", (long) res);

   exit(EXIT_SUCCESS);
}
