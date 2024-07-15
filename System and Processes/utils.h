/* System and Processes */

#ifndef SYSTEM_PROCESSES_H
#define SYSTEM_PROCESSES_H

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <unistd.h>
#include <limits.h>
#include <setjmp.h>
#include <pwd.h>
#include <fcntl.h>
#include <sys/utsname.h>
#include <grp.h>
#include <sys/fsuid.h>
#include <errno.h>
#include <string.h>
#include <ctype.h>
#include <shadow.h>

void raiseError(char *err_msg, char *err_file, int err_line)
{
   fprintf(stderr, "\n*** %s (%s::%d) ***\n", err_msg, err_file, err_line);
   exit(EXIT_SUCCESS); 
}

#define raiseError(err_msg) (raiseError(err_msg, __FILE__, __LINE__))

#endif /* SYSTEM_PROCESSES_H */
