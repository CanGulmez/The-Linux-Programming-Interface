/* Demonstrates the effect of timezones and locales */

#include "./utils.h"

void main(int argc, char *argv[])
{
   time_t t;
   struct tm *loc;
   char buffer[BUFSIZ];

   if (setlocale(LC_ALL, "") == NULL)
      raiseError("setlocale()");  /* Use locale settings in conversions */

   t = time(NULL);

   printf("ctime() of time() value is: %s\n", ctime(&t));

   loc = localtime(&t); 
   if (loc == NULL)
      raiseError("localtime()");

   printf("asctime of local time is: %s", asctime(loc));

   if (strftime(buffer, BUFSIZ, "%A, %d %B %Y, %H:%M:%S %Z", loc) == 0)
      raiseError("strftime()");

   printf("strftime() of local time is: %s\n", buffer);

   exit(EXIT_SUCCESS);
}
