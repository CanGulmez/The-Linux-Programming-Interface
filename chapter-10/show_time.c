/* Demonstrate the effect of timezones and locales */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <time.h>
#include <locale.h>
#include <assert.h>
#include <string.h>

#define BUF_SIZE     200

void main(int argc, char *argv[])
{
   time_t t;
   struct tm *loc;
   char buf[BUF_SIZE]; 

   assert (setlocale(LC_ALL, "") != NULL); // Use locale settings 
                                           // in conversions
   t = time(NULL);

   printf("ctime() of time() value is: %s", ctime(&t));

   loc = localtime(&t);
   assert (loc != NULL);

   printf("asctime() of local time is: %s", asctime(loc));

   assert (strftime(buf, BUF_SIZE, "%A, %d %B %Y, %H:%M:%S %Z", loc) 
           != 0);
   printf("strftime() of local time is: %s\n", buf);

   exit(EXIT_SUCCESS);
}
