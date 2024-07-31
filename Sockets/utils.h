/* SOCKETS */

#ifndef SOCKETS_H
#define SOCKETS_H

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <sys/types.h>
#include <errno.h>
#include <ctype.h>
#include <string.h>
#include <sys/socket.h>
#include <arpa/inet.h>
#include <sys/un.h>
#include <netdb.h>
#include <signal.h>
#include <netinet/in.h>

void xxx(char *err_msg, char *err_file, int err_line)
{
   fprintf(stderr, "\n*** %s (%s::%d) ***\n", err_msg, err_file, err_line);
   exit(EXIT_FAILURE); 
}

#define raiseError(err_msg) (xxx(err_msg, __FILE__, __LINE__))

#endif /* SOCKETS_H */
