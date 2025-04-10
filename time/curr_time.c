/* A function that returns a string containing the current time */

#include "../linux.h"

#define BUF_SIZE  1000

/* Return a string containing the current time formatted according to
   the specification in 'format' (see strftime(3) for specifiers).
   If 'format' is NULL, we use "%c" as a specifier (which gives the
   date and time as for ctime(3), but without the trailing newline).
   Returns NULL on error. */

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
