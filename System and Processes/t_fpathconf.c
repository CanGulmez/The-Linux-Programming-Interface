/* The program shows the use of 'fpathconf()' to retrieve various limits 
   for the file reffered to by its standard input. When we run this program
   specifying standard input as directory on an 'ext2' file system. 
*/

#include "./utils.h"

/* Print 'msg' plus value of fpathconf(fd, name) */
void fpathconfPrint(const char *msg, int fd, int name)
{
   long lim;

   errno = 0;
   lim = fpathconf(fd, name);

   if (lim != -1)                /* Call succeeded, limit determinate */
      printf("%s %ld\n", msg, lim);
   else
   {
      if (errno == 0)            /* Call succeeded, limit indeterminate */
         printf("%s (indeterminate)\n", msg);
      else                       /* Call failed */
         raiseError("fpathconf() did not work");
   }
}

void main(int argc, char *argv[])
{
   fpathconfPrint("_PC_NAME_MAX: ", STDIN_FILENO, _PC_NAME_MAX);
   fpathconfPrint("_PC_PATH_MAX: ", STDIN_FILENO, _PC_PATH_MAX);
   fpathconfPrint("_PC_PIPE_MAX: ", STDIN_FILENO, _PC_PIPE_BUF);

   exit(EXIT_SUCCESS); 
}
