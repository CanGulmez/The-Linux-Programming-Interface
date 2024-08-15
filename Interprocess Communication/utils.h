/* Interprocess Communication (IPS) */

#define _GNU_SOURCE     /* Include all standard-dependent definations */
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <signal.h>
#include <errno.h>
#include <sys/wait.h>
#include <time.h>
#include <ctype.h>
#include <fcntl.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <sys/time.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <sys/ipc.h>
#include <sys/file.h>
#include <mqueue.h>
#include <semaphore.h>
#include <sys/mman.h>

#ifndef IPS_H
#define IPS_H

void xxx(char* err_msg, char *err_file, int err_line)
{
   fprintf(stderr, "*** %s (%s::%d) ***\n", err_msg, err_file, err_line);
   exit(EXIT_FAILURE);
}

#define raiseError(err_msg) (xxx(err_msg, __FILE__, __LINE__))

#endif /* IPS_H */
