/* Using ''flock() */

#include "./utils.h"

char *currTime(const char *format)
{
   static char buffer[BUFSIZ];      /* Noreentrant */
   time_t t;
   size_t s;
   struct tm *tm;

   t = time(NULL);
   tm = localtime(&t);
   if (tm == NULL)   
      return NULL;

   s = strftime(buffer, BUFSIZ, (format != NULL) ? format : "%c", tm);

   return (s == 0) ? NULL : buffer;
}

void main(int argc, char *argv[])
{
   int fd, lock;
   const char *lname;

   if (argc < 3 || strcmp(argv[1], "--help") == 0 ||
       strchr("sx", argv[2][0]) == NULL)
      raiseError("prog-name file lock [sleep-time]\n"
               "  'lock' is 's' (shared) or 'x' (exclusive)\n"
               "     optionally followed by 'n' (nonblocking)\n"
               "  'secs' specifies time to hold lock\n");

   lock = (argv[2][0] == 's') ? LOCK_SH : LOCK_EX;
   if (argv[2][1] == 'n')
      lock |= LOCK_NB;

   fd = open(argv[1], O_RDONLY);       /* Open file to be locked */
   if (fd == -1)
      raiseError("open()");
   
   lname = (lock & LOCK_SH) ? "LOCK_SH" : "LOCK_EX";

   printf("PID %ld: requesting %s at %s\n", (long) getpid(), lname,
          currTime("%T"));

   if (flock(fd, lock) == -1) {
      if (errno == EWOULDBLOCK)
         raiseError("file already locked");
      else
         raiseError("flock()");
   }

   printf("PID %ld: granted    %s at %s\n", (long) getpid(), lname,
          currTime("%T"));

   sleep((argc > 3) ? atoi(argv[3]) : 10);

   printf("PID %ld: releasing %s at %s\n", (long) getpid(), lname,
          currTime("%T"));

   if (flock(fd, LOCK_UN) == -1)
      raiseError("flock()");

   exit(EXIT_SUCCESS);
}
