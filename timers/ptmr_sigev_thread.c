/* POSIX timer notification using a thread function */

#include "../linux.h"

#define BUF_SIZE  1000

static pthread_mutex_t mtx = PTHREAD_MUTEX_INITIALIZER;
static pthread_cond_t cond = PTHREAD_COND_INITIALIZER;

static int expireCnt = 0;     /* Number of expirations of all timers */

char *currTime(const char *format)
{
   static char buffer[BUF_SIZE];
   time_t t;
   size_t s;
   struct tm *tm;

   t = time(NULL);
   tm = localtime(&t);
   if (tm == NULL)
      return NULL;

   s = strftime(buffer, BUF_SIZE, (format != NULL) ? format : "%c", tm);
   
   return (s == 0) ? NULL : buffer;
}

static void threadFunc(union sigval sv)   /* Thread notification function */
{
   timer_t *tidptr;
   int s;

   tidptr = sv.sival_ptr;

   printf("[%s Thread notify]\n", currTime("%T"));
   printf("    timer ID=%ld\n", (long) *tidptr);
   printf("    timer_getoverun()=%d\n", timer_getoverrun(*tidptr));

   /* Increment counter variable shared with main thread and signal
      condition variable to notify main thread of the change. */

   s = pthread_mutex_lock(&mtx);
   if (s != 0)
      syscall_error();

   expireCnt += 1 + timer_getoverrun(*tidptr);

   s = pthread_mutex_unlock(&mtx);
   if (s != 0)
      syscall_error();

   s = pthread_cond_signal(&cond);
   if (s != 0)
      syscall_error();
}

void itimerspecFromStr(char *str, struct itimerspec *tsp)
{
   char *cptr, *sptr;

   cptr = strchr(str, ':');
   if (cptr != NULL)
      *cptr = '\0';

   sptr = strchr(str, '/');
   if (sptr != NULL)
      *sptr = '\0';
 
   tsp->it_value.tv_sec = atoi(str);
   tsp->it_value.tv_nsec = (sptr != NULL) ? atoi(sptr + 1) : 0;

   if (cptr == NULL) {
      tsp->it_interval.tv_sec = 0;
      tsp->it_interval.tv_nsec = 0;
   } else {
      sptr = strchr(cptr + 1, '/');
      if (sptr != NULL)
         *sptr = '\0';
      tsp->it_interval.tv_sec = atoi(cptr + 1);
      tsp->it_interval.tv_nsec = (sptr != NULL) ? atoi(sptr + 1) : 0;
   }
}

void main(int argc, char *argv[])
{
   struct sigevent sev;
   struct itimerspec ts;
   timer_t *tidlist;
   int s, j;  

   if (argc < 2)
      usage_error("Wrong command-line usage");

   tidlist = calloc(argc - 1, sizeof(timer_t));
   if (tidlist == NULL)
      syscall_error();

   sev.sigev_notify = SIGEV_THREAD;          /* Notify via thread */
   sev.sigev_notify_function = threadFunc;   /* Thread start function */
   sev.sigev_notify_attributes = NULL;

   /* Create and start one timer for each command-line argument */

   for (j = 0; j < argc - 1; j++) {
      itimerspecFromStr(argv[j + 1], &ts);

      sev.sigev_value.sival_ptr = &tidlist[j];  /* Passed as argument to threadFunc() */

      if (timer_create(CLOCK_REALTIME, &sev, &tidlist[j]) == -1)
         syscall_error();
      printf("Timer ID: %ld (%s)\n", (long) tidlist[j], argv[j + 1]);

      if (timer_settime(tidlist[j], 0, &ts, NULL) == -1)
         syscall_error();
   }

   /* The main thread waits on a condition variable that is signaled
      on each invocation of the thread notification function. We
      print a message so that user can see that this occured. */

   s = pthread_mutex_lock(&mtx);
   if (s != 0)
      syscall_error();

   for (;;) {
      s = pthread_cond_wait(&cond, &mtx);
      if (s != 0)
         syscall_error();
      printf("main(): expireCnt = %ld\n", expireCnt);
   }
}
