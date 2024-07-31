/* POSIX Threads */

#ifndef POSIX_THREADS_H
#define POSIX_THREADS_H

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <time.h>
#include <fcntl.h>
#include <pthread.h>

void xxx(char *err_msg, char *err_file, int err_line)
{
   fprintf(stderr, "\n*** %s (%s::%d)***\n", err_msg, err_file, err_line);
   exit(EXIT_FAILURE);
}

#define destroyProgram(err_msg) (xxx(err_msg, __FILE__, __LINE__))

#endif /* POSIX_THREADS_H */
