/* This program demonstrates the use of 'strptime()' and 'strftime()'.
   That takes a command-line argument containing a date and time, 
   converts this to a broken-down time using strptime(), and then 
   displays the results of performing the reverse conversion using
   'strftime()'. The program takes up to three arguments, of which
   the first two are required. The first argument is the string 
   containing a date and time. The second argument is the format
   specfication to be used by strptime() to parse the first argument.
   The optional third argument is the format string to be used by
   'strftime()' for the reverse conversion. If this argument is 
   omitted, a default format string is used. */

#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <locale.h>
#include <time.h>
#include <assert.h>
#include <string.h>

#define SBUF_SIZE    1000

void main(int argc, char *argv[])
{
   struct tm tm;
   char sbuf[SBUF_SIZE];
   char *ofmt;

   assert (argc >= 3 && strcmp(argv[1], "--help"));

   assert (setlocale(LC_ALL, "") != NULL); // Use locale settings
                                           // in conversions
   memset(&tm, 0, sizeof(struct tm));  // Initialize 'tm'

   assert (strptime(argv[1], argv[2], &tm) != NULL);

   tm.tm_isdst = -1;    // Note set by strptime(); tells mktime()
                        // to determine if DST is in effect
   printf("Calender time (seconds since Epoch): %ld\n", 
          (long) mktime(&tm));

   ofmt = (argc > 3) ? argv[3] : "%H:%M:%S %A, %d %B %Y %Z";

   assert (strftime(sbuf, SBUF_SIZE, ofmt, &tm) != 0);

   printf("strftime() yields: %s\n", sbuf);
   
   exit(EXIT_SUCCESS);
}
