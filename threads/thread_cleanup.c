/* Using cleanup handlers */

#include "../linux.h"

static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
static int glob = 0;

/* Free memory pointed to by 'arg' and unlock mutex */
static void cleanupHandler(void *arg)
{
   int s;

   printf("cleanup: freeing block at %p\n", arg);
   free(arg);

   printf("cleanup: unlocking mutex\n");
   s = pthread_mutex_unlock(&mtx);
   if (s != 0)
      syscall_error();
}

static void *threadFunc(void *arg)
{
   int s;
   void *buf = NULL;          /* Buffer allocated by thread */

   buf = malloc(0x10000);     /* Not a cancellation point */
   printf("thread:  allocated memory at %p\n", buf);

   s = pthread_mutex_lock(&mtx); /* Not a cancellation point */
   if (s != 0)
      syscall_error();

   pthread_cleanup_push(cleanupHandler, buf);

   while (glob == 0) {
      s = pthread_cond_wait(&cond, &mtx); /* A cancellation point */
      if (s != 0)
         syscall_error();
   }

   printf("thread:  condition wait loop completed\n");
   pthread_cleanup_pop(1);       /* Executes cleanup handler */
   
   return NULL;
}

void main(int argc, char *argv[])
{
   pthread_t thread;
   void *res;
   int s;

   s = pthread_create(&thread, NULL, threadFunc, NULL);
   if (s != 0)
      syscall_error();

   sleep(2);                     /* Give thread a change to get started */

   if (argc == 1) {              /* Cancel thread */
      printf("main:  about to cancel thread\n");
      s = pthread_cancel(thread);
      if (s != 0)
         syscall_error();
   } else {                      /* Signal condition variable */
      printf("main:  about to signal condition variable\n");
      glob = 1;
      s = pthread_cond_signal(&cond);
      if (s != 0)
         syscall_error();
   }

   s = pthread_join(thread, &res);
   if (s != 0)
      syscall_error();
   
   if (res == PTHREAD_CANCELED)
      printf("main:  thread was canceled\n");
   else
      printf("main:  thread terminated normally\n");

   exit(EXIT_SUCCESS);
}
