/* Signals */

#ifndef SIGNALS_H
#define SIGNALS_H

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <errno.h>
#include <string.h>
#include <signal.h>
#include <time.h>

void raiseError(char *err_msg, char *err_file, int err_line)
{
   fprintf(stderr, "\n*** %s (%s::%d) ***\n", err_msg, err_file, err_line);
   exit(EXIT_SUCCESS);  
}

#define raiseError(err_msg) (raiseError(err_msg, __FILE__, __LINE__))

#endif /* SIGNALS_H */
