/* Canceling a thread with 'pthread_cancel()' */

#include "../linux.h"

static void *threadFunc(void *arg)
{
   int i;

   printf("New thread started\n");  /* May be a cancellation point */
   for (i = 0; ; i++) {
      printf("Loop %d\n", i);       /* May be a cancellation point */
      sleep(1);                     /* A cancellation point */
   }

   return NULL;
}

void main(int argc, char *argv[])
{
   pthread_t thread;
   int s;
   void *res;

   s = pthread_create(&thread, NULL, threadFunc, NULL);
   if (s != 0)
      syscall_error();

   sleep(3);                        /* Allow new thread to run a while */

   s = pthread_cancel(thread);
   if (s != 0)
      syscall_error();

   s = pthread_join(thread, &res);
   if (s != 0)
      syscall_error();

   if (res == PTHREAD_CANCELED)
      printf("Thread was canceled\n");
   else
      printf("Thread was not canceled (should not happen!)\n");

   exit(EXIT_SUCCESS);
}
