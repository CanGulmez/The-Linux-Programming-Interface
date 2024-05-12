/* This program demonstrates how to read and modify a /proc file. 
   This program reads and displays the contents of /proc/sys/kernel/
   pid_max. If a command-line argument is supplied, the program 
   updates the file using that value. This file specifies an upper
   limit for process IDs. */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <fcntl.h>

#define MAX_LINE     100

void main(int argc, char *argv[])
{
   int fd;
   char line[MAX_LINE];
   ssize_t n;

   fd = open("/proc/sys/kernel/pid_max", (argc > 1):O_RDWR?O_RDONLY);
   assert (fd != -1);

   n = read(fd, line, MAX_LINE);
   assert (n != -1);

   if (argc > 1)
      printf("Old value: ");
   printf("%.*s", (int) n, line);

   if (argc > 1)
      assert (write(fd, argv[1], strlen(argv[1])) == strlen(argv[1])),
      system("echo /proc/sys/kernel/pid_max now contains "
             "`cat /proc/sys/kernel/pid_max`");

   exit(EXIT_SUCCESS);
}
