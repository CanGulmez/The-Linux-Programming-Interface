/* System and Processes */

#ifndef SYSTEM_PROCESSES_H
#define SYSTEM_PROCESSES_H

#define _GNU_SOURCE
#include <stdio.h>
#include <syslog.h>
#include <stdlib.h>
#include <unistd.h>
#include <unistd.h>
#include <paths.h>
#include <sys/ioctl.h>
#include <utmpx.h>
#include <limits.h>
#include <sys/capability.h>
#include <setjmp.h>
#include <pwd.h>
#include <fcntl.h>
#include <sys/utsname.h>
#include <grp.h>
#include <sys/stat.h>
#include <sys/fsuid.h>
#include <sys/wait.h>
#include <termio.h>
#include <errno.h>
#include <string.h>
#include <sys/types.h>
#include <ctype.h>
#include <sys/resource.h>
#include <sched.h>
#include <shadow.h>
#include <time.h>
#include <sys/time.h>
#include <sys/acct.h>

void xxx(char *err_msg, char *err_file, int err_line)
{
   fprintf(stderr, "\n*** %s (%s::%d) ***\n", err_msg, err_file, err_line);
   exit(EXIT_SUCCESS);  
}

#define raiseError(err_msg) (xxx(err_msg, __FILE__, __LINE__))

#endif /* SYSTEM_PROCESSES_H */
