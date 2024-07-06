/* Utils for The Linux Programming Interface */

#define _GNU_SOURCE     /* Include all standard-dependent definations */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <malloc.h>
#include <sys/types.h>
#include <errno.h>
#include <limits.h>
#include <dirent.h>
#include <ftw.h>
#include <ctype.h>
#include <pwd.h>
#include <grp.h>
#include <libgen.h>
#include <getopt.h>
#include <sys/mount.h>
#include <sys/uio.h>
#include <time.h>
#include <sys/xattr.h>

#ifndef LINUX_INTERFACE_H
#define LINUX_INTERFACE_H

#define file __FILE__
#define line __LINE__

void raiseError(char *err_msg, char *err_file, int err_line)
{
   fprintf(stderr, "*** %s! (%s:%d) ***\n", err_msg, err_file, err_line);
   exit(EXIT_FAILURE); 
}

#endif /* LINUX_INTERFACE_H */
