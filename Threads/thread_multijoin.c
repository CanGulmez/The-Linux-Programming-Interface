/* A main thread that can join with any terminated thread */

#include "./utils.h"

static pthread_cond_t threadDied = PTHREAD_COND_INITIALIZER;
static pthread_mutex_t threadMutex = PTHREAD_MUTEX_INITIALIZER;
            /* Protects all of the following global variables */

static int totThreads = 0;    /* Total number of threads created */
static int numLive = 0;       /* Total number of threads still alive or
                                 terminated but not yet joined */
static int numUnjoined = 0;   /* Number of terminated threads that 
                                 have not yet been joined */
enum tstate {                 /* Thread states */  
   TS_ALIVE,                  /* Thread is alive */ 
   TS_TERMINATED,             /* Thread terminated, not yet joined */
   TS_JOINED                  /* Thread terminated, and joined */
};

static struct {               /* Info about each thread */
   pthread_t tid;             /* OD of this thread */
   enum tstate state;         /* Thread state (TS_* constants above) */
   int sleepTime;             /* Number of seconds to live before terminating */
} *thread;

static void *threadFunc(void *arg)  /* Start function for thread */
{
   int idx = *((int *) arg);
   int s;

   sleep(thread[idx].sleepTime);    /* Simulate doing some work */
   printf("Thread %d terminating\n", idx);

   s = pthread_mutex_lock(&threadMutex);
   if (s != 0)
      destroyProgram("pthread_mutex_lock()");

   numUnjoined ++;
   thread[idx].state = TS_TERMINATED;

   s = pthread_mutex_unlock(&threadMutex);
   if (s != 0)
      destroyProgram("pthread_mutex_unlock()");

   s = pthread_cond_signal(&threadDied);
   if (s != 0)
      destroyProgram("pthread_cond_signal()");

   return NULL;
}

void main(int argc, char *argv[])
{
   int s, idx;

   if (argc < 2 || !strcmp(argv[1], "--help"))
      destroyProgram("wrong command-line arguments");

   thread = calloc(argc - 1, sizeof(*thread));
   if (thread == NULL)
      destroyProgram("calloc()");

   /* Create all threads */

   for (idx = 0; idx < argc - 1; idx ++)
   {
      thread[idx].sleepTime = atoi(argv[idx + 1]);
      thread[idx].state = TS_ALIVE;
      
      s = pthread_create(&thread[idx].tid, NULL, threadFunc, &idx);
      if (s != 0)
         destroyProgram("pthread_create()");
   }

   totThreads = argc - 1;
   numLive = totThreads;

   /* Joing with terminated threads */

   while (numLive > 0)
   {
      s = pthread_mutex_lock(&threadMutex);
      if (s != 0)
         destroyProgram("pthread_mutex_lock()");

      while (numUnjoined == 0)
      {
         s = pthread_cond_wait(&threadDied, &threadMutex);
         if (s != 0)
            destroyProgram("pthread_cond_wait()");
      }

      for (idx = 0; idx < totThreads; idx ++)  
      {
         if (thread[idx].state == TS_TERMINATED)
         {
            s = pthread_join(thread[idx].tid, NULL);
            if (s != 0)
               destroyProgram("pthread_join()");

            thread[idx].state = TS_JOINED; 
            numLive --;
            numUnjoined --; 

            printf("Reaped thread %d (numLive=%d)\n", idx, numLive);
         }
      }
      s = pthread_mutex_unlock(&threadMutex);
      if (s != 0)
         destroyProgram("pthread_mutex_unlock()");
   }
   exit(EXIT_SUCCESS);
}
