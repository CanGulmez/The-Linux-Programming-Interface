/* Retrieving and converting calendar times */

#include "../linux.h"

#define SBUF_SIZE       1000

void main(int argc, char *argv[])
{
   struct tm tm;
   char sbuffer[SBUF_SIZE];
   char *ofmt;

   if (argc < 3 || strcmp(argv[1], "--help") == 0)
      usage_error("Wrong command-line usage");

   if (setlocale(LC_ALL, "") == NULL) 
      syscall_error();  /* Use locale settings in conversions */

   memset(&tm, 0, sizeof(struct tm));  /* Initialize 'tm' */
   if (strptime(argv[1], argv[2], &tm) == NULL)
      syscall_error();

   tm.tm_isdst = -1;    /* Not set by strptime(); tells mktime()
                           to determine if DST is in effect */
   printf("Calendar time (seconds since Epoch): %ld\n", (long) mktime(&tm));

   ofmt = (argc > 3) ? argv[3] : "%H:%M:%S %A, %d %B %Y %Z";
   if (strftime(sbuffer, SBUF_SIZE, ofmt, &tm) == 0)
      syscall_error();
   printf("strftime() yields: %s\n", sbuffer);

   exit(EXIT_SUCCESS);
}
