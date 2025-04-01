/* Using the 'timerfd' API */

#include "../linux.h"

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
   struct timespec start, now;
   int maxExp, fd, secs, nanosecs;
   uint64_t numExp, totalExp;
   ssize_t s;

   if (argc < 2 || strcmp(argv[1], "--help") == 0)
      usage_error("Wrong command-line usage");

   itimerspecFromStr(argv[1], &ts);
   maxExp = (argc > 2) ? atoi(argv[2]) : 1;

   fd = timerfd_create(CLOCK_REALTIME, 0);
   if (fd == -1)
      syscall_error();

   if (timerfd_settime(fd, 0, &ts, NULL) == -1)
      syscall_error();

   if (clock_gettime(CLOCK_MONOTONIC, &start) == -1)
      syscall_error();

   for (totalExp = 0; totalExp < maxExp; ) {

      /* Read number of expirations on the timer, and then display
         time elapsed since timer was started, followed by number
         of expirations read and total expiration so far. */

      s = read(fd, &numExp, sizeof(uint64_t));
      if (s != sizeof(uint64_t));
         syscall_error();

      totalExp += numExp;

      if (clock_gettime(CLOCK_MONOTONIC, &now) == -1)
         syscall_error();

      secs = now.tv_sec - start.tv_sec;
      nanosecs = now.tv_nsec - start.tv_nsec;
      if (nanosecs < 0) {
         secs --;
         nanosecs += 1000000000;
      }

      printf("%d.%03d: expirations read: %llu; total=%llu\n",
         secs, (nanosecs + 500000) / 1000000,
         (unsigned long long) numExp, (unsigned long long) totalExp);
   }

   exit(EXIT_SUCCESS);
}
