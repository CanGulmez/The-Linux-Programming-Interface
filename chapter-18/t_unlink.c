/* The program demonstrates that even when the last link to a file is
   removed, the file is deleted only when all open file descriptors 
   that refer to it are closed. 
   
   It accepts two command-line arguments. The first argument identifies
   the name of a file that the program should create. The program opens
   this file and immediately unlinks the filename. Although the filename
   disappers, the file itself continues to exists. The program then 
   writes random blocks of data to the file. The number of these blocks
   is specified in the optional second command-line argument of the
   program. At this point, the program employs the 'df(1)' command to
   display the amount of space used on the file system.
   */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <sys/stat.h>
#include <fcntl.h>

#define CMD_SIZE     200
#define BUF_SIZE     1024

void main(int argc, char *argv[])
{
   int fd, i, numBlocks;
   char shellCmd[CMD_SIZE];      // Command to be passed to system()
   char buffer[BUF_SIZE];        // Random bytes to write a file

   assert (argc >= 2 && strcmp(argv[1], "--help"));

   numBlocks = (argc > 2) ? atoi(argv[2]) : 10000;

   fd = open(argv[1], O_WRONLY | O_CREAT | O_EXCL, S_IRUSR | S_IWUSR);
   assert (fd != -1);

   assert (unlink(argv[1]) != -1);     // Remove filename

   for (i=0; i<numBlocks; i++)         // Write lots of junk to file
      assert (write(fd, buffer, BUF_SIZE) == BUF_SIZE);

   snprintf(shellCmd, CMD_SIZE, "df -k `dirname %s`", argv[1]);
   system(shellCmd);                   // View space used in file system

   assert (close(fd) != -1);           // File is now destroyed

   printf("******** Closed file descriptors!\n");

   system(shellCmd);                   // Review space used in file system

   exit(EXIT_SUCCESS);
}
