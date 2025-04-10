/* Demonstrate the effect of timezones and locals */

#include "../linux.h"

#define BUF_SIZE     200

void main(int argc, char *argv[])
{
   time_t t;
   struct tm *loc;
   char buf[BUF_SIZE];

   if (setlocale(LC_ALL, "") == NULL)
      syscall_error();     /* Use locale settings in conversions */

   t = time(NULL);

   printf("ctime() of time() value it: %s", ctime(&t));

   loc = localtime(&t);
   if (loc == NULL)
      syscall_error();

   printf("asctime() of local time is: %s", asctime(loc));

   if (strftime(buf, BUF_SIZE, "%A, %d %B %Y, %H:%M:%S %Z", loc) == 0)
      syscall_error();
   printf("strftime() of local time is: %s\n", buf);

   exit(EXIT_SUCCESS);
}
