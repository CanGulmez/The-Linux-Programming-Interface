/* Using 'nanosleep()' */

#include "../linux.h"

static void sigintHandler(int sig)
{
   return;           /* Just interrupt nanosleep() */
}

void main(int argc, char *argv[])
{
   struct timeval start, finish;
   struct timespec request, remain;
   struct sigaction sa;
   int s;

   if (argc != 3 || strcmp(argv[1], "--help") == 0)
      usage_error("Wrong command-line usage");

   request.tv_sec = atol(argv[1]);
   request.tv_nsec = atol(argv[2]);

   /* Allow SIGINT handler to interrupt nanosleep() */

   sigemptyset(&sa.sa_mask);
   sa.sa_flags = 0;
   sa.sa_handler = sigintHandler;
   if (sigaction(SIGINT, &sa, NULL) == -1)
      syscall_error();

   if (gettimeofday(&start, NULL) == -1)
      syscall_error();

   for (;;) {
      s = nanosleep(&request, &remain);
      if (s == -1 && errno != EINTR)
         syscall_error();

      if (gettimeofday(&finish, NULL) == -1)
         syscall_error();
      printf("Slept for: %9.6f secs\n", finish.tv_sec - start.tv_sec + 
         (finish.tv_usec - start.tv_usec) / 1000000.0);

      if (s == 0)
         break;            /* nanosleep() completed */

      printf("Remaining: %2ld.%09ld\n", (long) remain.tv_sec, 
         remain.tv_nsec);

      request = remain;
   }
   printf("Sleep complete\n");
   exit(EXIT_SUCCESS);
}
