/* A function that returns a string containing the current time */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <assert.h>
#include <string.h>

#define BUF_SIZE     1000

// Return a string containing the current time formatted according to
// the specification in 'format' (see strftime(3) for specifiers). If
// 'format' is NULL, we use "%c" as a specifier (which gives the date
// and time as for ctime(3), but without the trailing newline). 
// Return NULL on error.

char *currTime(const char *format)
{
   static char buf[BUF_SIZE];       // Nonreentrant
   time_t t;
   ssize_t s;
   struct tm *tm;
   _POSIX_
   t = time(NULL);
   tm = localtime(&t);  

   assert (tm != NULL); 
   
   s = strftime(buf, BUF_SIZE, (format != NULL) ? format : "%c", tm);

   return (s == 0) ? NULL : buf;
}
