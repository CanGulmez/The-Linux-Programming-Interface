/* Memory Allocation */

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>

#ifndef MEMORY_ALLOCATION_H
#define MEMORY_ALLOCATION_H

void raiseError(char *err_msg, char *err_file, int err_line)
{
   fprintf(stderr, "\n*** %s (%s::%d) ***\n", err_msg, err_file, err_line);
   exit(EXIT_FAILURE);
}

#define raiseError(err_msg) (raiseError(err_msg, __FILE__, __LINE__))

#endif /* MEMORY_ALLOCATION_H */
