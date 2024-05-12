/* The program shows the use of fpathname() to retrieve various 
   limits for the file referred to by its standard input. When we
   run this program specifying standard input as a directory on 
   an ext file system, we can see its outputs. */ 

#include <stdio.h>
#include <stdlib.h>
#include <assert.h>
#include <string.h>
#include <unistd.h>
#include <errno.h>

/* Print 'msg' plus value of fpathconf(fd, name) */
static void fpathconfPrint(const char *msg, int fd, int name)
{
   long lim;

   errno = 0; 
   lim = fpathconf(fd, name);
   if (lim != -1)             // Call succeeded, limit determinate
      printf("%s %ld\n", msg, lim);
   else
      if (errno == 0)         // Call succeeded, limit indeterminate
         printf("%s (indeterminate)\n", msg);
      else                    // Call failed
         abort();
}

void main(int argc, char *argv[])
{
   fpathconfPrint("_PC_NAME_MAX: ", STDIN_FILENO, _PC_NAME_MAX);
   fpathconfPrint("_PC_PATH_MAX: ", STDIN_FILENO, _PC_PATH_MAX);
   fpathconfPrint("_PC_PIPE_BUF: ", STDIN_FILENO, _PC_PIPE_BUF);

   exit(EXIT_SUCCESS); 
}
