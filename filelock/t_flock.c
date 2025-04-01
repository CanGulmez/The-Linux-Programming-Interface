/* Using 'flock()' */

#include "../linux.h"

#define BUF_SIZE  1000

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


void main(int argc, char *argv[])
{
   int fd, lock;
   const char *lname;

   if (argc < 3 || strcmp(argv[1], "--help") == 0)
      usage_error("Wrong command-line usage");

   lock = (argv[2][0] == 's') ? LOCK_SH : LOCK_EX;
   if (argv[2][1] == 'n')
      lock |= LOCK_NB;

   fd = open(argv[1], O_RDONLY);
   if (fd == -1)
      syscall_error();

   lname = (lock & LOCK_SH) ? "LOCK_SH" : "LOCK_EX";

   printf("PID %ld: requesting %s at %s\n", (long) getpid(), 
      lname, currTime("%T"));

   if (flock(fd, lock) == -1)
      syscall_error();

   printf("PID %ld: granted   %s at %s\n", (long) getpid(), lname,
      currTime("%T"));

   sleep((argc > 3) ? atoi(argv[3]) : 10);

   printf("PID %ld: release  %s at %s\n", (long) getpid(), lname,
      currTime("%T"));
   if (flock(fd, LOCK_UN) == -1)
      syscall_error();

   exit(EXIT_SUCCESS);
}
