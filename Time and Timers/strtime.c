/* The program demonstrates the use of 'strptime()' and 'strftime()'. This
   program takes a command-line argument containing a date and time, converts
   this to a break-down time using 'strptime()', and then displays the 
   result of performing the reverse conversion using 'strftime()'. 
*/

#include "./utils.h"

void main(int argc, char *argv[])
{
   struct tm tm;
   char sbuffer[BUFSIZ];
   char *ofmt;

   if (argc < 3 || !strcmp(argv[1], "--help"))
      raiseError("wrong command-line arguments");

   if (setlocale(LC_ALL, "") == NULL)
      raiseError("setlocale()");  /* Use locale settings in conversions */

   memset(&tm, 0, sizeof(struct tm));     /* Initialize 'tm' */
   if (strptime(argv[1], argv[2], &tm) == NULL)
      raiseError("strptime()");

   tm.tm_isdst = -1;    /* Not set by strptime(); tells mktime()
                           to determine if DST is in effect */
   printf("calender time (seconds since Epoch): %ld\n", (long) mktime(&tm));

   ofmt = (argc > 3) ? argv[3] : "%H:%M:%S %A, %d %B %Y %Z";
   if (strftime(sbuffer, BUFSIZ, ofmt, &tm) == 0)
      raiseError("strftime()");

   printf("strftime() yields: %s\n", sbuffer); 

   exit(EXIT_SUCCESS);
}
