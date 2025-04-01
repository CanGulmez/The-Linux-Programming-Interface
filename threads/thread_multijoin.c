/* A main thread that can join with any terminated thread */

#include "../linux.h"

static pthread_cond_t threadDied = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t threadMutex = PTHREAD_MUTEX_INITIALIZER;
         /* Protects all of the following global variables */

static int totThreads = 0;    /* Total number of threads created */
static int numLive = 0;       /* Total number of threads still alive 
                                 or terminated but not yet joined */
static int numUnjoined = 0;   /* Number of terminated threads that
                                 have not yet been joined */
enum tstate {                 /* Thread states */
   TS_ALIVE,                  /* Thread is alive */
   TS_TERMINATED,             /* Thread terminated, not yet joined */
   TS_JOINED                  /* Thread terminated, and joined */
};

static struct {               /* Info about each thread */
   pthread_t tid;             /* ID of this thread */
   enum tstate state;         /* Thread state */
   int sleepTime              /* Number of seconds to live before terminating */
} *thread;

static void *threadFunc(void *arg)
{
   int idx = *((int *) arg);
   int s;

   sleep(thread[idx].sleepTime);
   printf("Thread %d terminating\n", idx);

   s = pthread_mutex_lock(&threadMutex);
   if (s != 0)
      syscall_error();

   numUnjoined ++;
   thread[idx].state = TS_TERMINATED;

   s = pthread_mutex_unlock(&threadMutex);
   if (s != 0)
      syscall_error();

   s = pthread_cond_signal(&threadDied);
   if (s != 0)
      syscall_error();

   return NULL;
}

void main(int argc, char *argv[])
{
   int s, idx;

   if (argc < 2 || strcmp(argv[1], "--help") == 0)
      usage_error("Wrong command-line usage");

   thread = calloc(argc - 1, sizeof(*thread));
   if (thread == NULL)
      syscall_error();
   
   /* Create all threads */

   for (idx = 0; idx < argc - 1; idx++) {
      thread[idx].sleepTime = atoi(argv[idx + 1]);
      thread[idx].state = TS_ALIVE;
      s = pthread_create(&thread[idx].tid, NULL, threadFunc, &idx);
      if (s != 0)
         syscall_error();
   }

   totThreads = argc - 1;
   numLive = totThreads;

   /* Join with terminated threads */

   while (numLive > 0) {
      s = pthread_mutex_lock(&threadMutex);
      if (s != 0)
         syscall_error();

      while (numUnjoined == 0) {
         s = pthread_cond_wait(&threadDied, &threadMutex);
         if (s != 0)
            syscall_error();
      }

      for (idx = 0; idx < totThreads; idx ++) {
         if (thread[idx].state == TS_TERMINATED) {
            s = pthread_join(thread[idx].tid, NULL);
            if (s != 0)
               syscall_error();

            thread[idx].state = TS_JOINED;
            numLive --;
            numUnjoined --;

            printf("Reaped thread %d (numLive=%d)\n", idx, numLive);
         }
      }
      s = pthread_mutex_unlock(&threadMutex);
      if (s != 0)
         syscall_error();
   }

   exit(EXIT_SUCCESS);
}
