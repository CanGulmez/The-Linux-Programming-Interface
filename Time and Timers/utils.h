/* Time and Timers */

#ifndef TIME_TIMERS_H
#define TIME_TIMERS_H

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <time.h>
#include <sys/time.h>
#include <sys/times.h>
#include <sys/timex.h>
#include <locale.h>

void raiseError(char *err_msg, char *err_file, int err_line)
{
   fprintf(stderr, "\n*** %s (%s::%d) ***\n", err_msg, err_file, err_line);
   exit(EXIT_SUCCESS); 
}

#define raiseError(err_msg) (raiseError(err_msg, __FILE__, __LINE__))

#endif /* TIME_TIMERS_H */