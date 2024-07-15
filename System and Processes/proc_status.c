/* THe program displays the process information using '/proc/PID' directory. 
   The first command-line argument defines the process ID that will be 
   displayed and must be defined.
*/

#include "./utils.h"

void main(int argc, char *argv[])
{
   if (argc != 2 || !strcmp(argv[1], "--help"))
      raiseError("wrong command-line argument");

   int fd;
   ssize_t numRead;
   char pathname[BUFSIZ], buffer[BUFSIZ];

   snprintf(pathname, BUFSIZ, "/proc/%d/status", atoi(argv[1]));

   fd = open(pathname, O_RDONLY);
   if (fd == -1)
      raiseError("open() did not work");

   numRead = read(fd, buffer, BUFSIZ);
   if (numRead == -1)
      raiseError("read() did not work");

   printf("%s", buffer);

   exit(EXIT_SUCCESS); 
}
