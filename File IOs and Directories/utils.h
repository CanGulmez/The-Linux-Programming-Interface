/* File IOs and Directories */

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
#include <sys/select.h>
#include <pwd.h>
#include <grp.h>
#include <poll.h>
#include <libgen.h>
#include <getopt.h>
#include <sys/mount.h>
#include <sys/uio.h>
#include <time.h>
#include <sys/xattr.h>
#include <sys/inotify.h>

#ifndef FILE_IOs_DIRECTORIES_H
#define FILE_IOs_DIRECTORIES_H

void xxx(char *err_msg, char *err_file, int err_line)
{
   fprintf(stderr, "\n*** %s (%s::%d) ***\n", err_msg, err_file, err_line);
   exit(EXIT_FAILURE); 
}

#define raiseError(err_msg) (xxx(err_msg, __FILE__, __LINE__))

#endif /* FILE_IOs_DIRECTORIES_H */
