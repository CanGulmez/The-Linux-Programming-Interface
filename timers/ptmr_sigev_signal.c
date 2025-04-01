/* POSIX timer notification using a signal */

#include "../linux.h"

#define BUF_SIZE  1000
#define TIMER_SIG SIGRTMIN    /* Our timer notification signal */

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

static void handler(int sig, siginfo_t *si, void *uc)
{
    timer_t *tidptr;

    tidptr = si->si_value.sival_ptr;

    printf("[%s] Got signal %d\n", curr_time("%T"), sig);
    printf("    *sival_ptr         = %ld\n", (long) *tidptr);
    printf("    timer_getoverrun() = %d\n", timer_getoverrun(*tidptr));
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
   struct itimerspec ts;
   struct sigaction sa;
   struct sigevent sev;
   timer_t *tidlist;
   int i;

   if (argc < 2)
      usage_error("Wrong command-line usage");
      
   tidlist = calloc(argc - 1, sizeof(timer_t));
   if (tidlist == NULL)
      syscall_error();

   /* Establish handler for notification signal */

   sa.sa_flags = SA_SIGINFO;
   sa.sa_sigaction = handler;
   sigemptyset(&sa.sa_mask);
   if (sigaction(TIMER_SIG, &sa, NULL) == -1)
      syscall_error();

   /* Create and start one timer for each command-line argument */

   sev.sigev_notify = SIGEV_SIGNAL;    /* Notify via signal */
   sev.sigev_signo = TIMER_SIG;        /* Notify using this signal */

   for (i = 0; i < argc - 1; i++) {
      itimerspecFromStr(argv[i + 1], &ts);

      sev.sigev_value.sival_ptr = &tidlist[i]; /* Allows handler to get ID of this timer */

      if (timer_create(CLOCK_REALTIME, &sev, &tidlist[i]) == -1)
         syscall_error();
      printf("Timer ID: %ld (%s)\n", (long) tidlist[i], argv[i + 1]);

      if (timer_settime(tidlist[i], 0, &ts, NULL) == -1)
         syscall_error();
   }

   for (;;)
      pause();                         /* Wait for incoming timer signals */
}
