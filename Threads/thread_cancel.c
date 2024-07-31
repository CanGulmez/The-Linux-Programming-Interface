/* Canceling a thread with 'pthread_cancel()'. */

#include "./utils.h"

static void *threadFunc(void *arg)
{
   int i;

   printf("New thread started\n");     /* May be a cancellation point */
   for (i=0; ; i++)
   {
      printf("Loop %d\n", i);          /* May be a cancellation point */
      sleep(1);
   }

   /* NOTREACHED */
   return NULL;
}

void main(int argc, char *argv[])
{
   pthread_t thr;
   int s;
   void *res;

   s = pthread_create(&thr, NULL, threadFunc, NULL);
   if (s != 0)
      destroyProgram("pthread_create()");

   sleep(3);                           /* Allow new thread to run a while */

   s = pthread_cancel(thr);
   if (s != 0)
      destroyProgram("pthread_cancel()");

   s = pthread_join(thr, &res);
   if (s != 0)
      destroyProgram("pthread_join()");

   if (res == PTHREAD_CANCELED)
      printf("Thread was canceled\n");
   else
      printf("Thread was not canceled (should not happen!)\n");

   exit(EXIT_SUCCESS);
}