/* The program demonstrates that even when the last link to a file is
   removed, the file is deleted only when all open file descriptors 
   that refer to it are closed. 
*/

#include "../utils.h"

#define CMD_SIZE     200
#define BUF_SIZE     1024  

void main(int argc, char *argv[]) 
{
   int fd, i, numBlocks;
   char shellCmd[CMD_SIZE];      /* Command to be passed to system() */
   char buf[BUF_SIZE];           /* Random bytes to write to file */

   if (argc < 2 || !strcmp(argv[1], "--help"))
      raiseError("wrong command-line arguments", file, line);

   numBlocks = (argc > 2) ? atoi(argv[2]) : 100000;

   fd = open(argv[1], O_WRONLY | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);
   if (fd == -1)
      raiseError("open() did not work", file, line);

   if (unlink(argv[1]) == -1)     /* Remove filename */
      raiseError("unlink() did not work", file, line);

   for (i=0; i<numBlocks; i++)         /* Write lots of junk to file */
      if (write(fd, buf, BUF_SIZE) != BUF_SIZE)
         raiseError("write() did not work", file, line);

   snprintf(shellCmd, CMD_SIZE, "df -k `dirname %s`", argv[1]);
   system(shellCmd);                   /* View space used in file system */

   if (close(fd) == -1)           /* File is now destroyed */
      raiseError("close() did not work", file, line);

   printf("****** Closed file descriptor\n"); 

   system(shellCmd);                   /* Review space used in file system */

   exit(EXIT_SUCCESS); 
}
