/* This program uses 'opendir()', 'readdir()', 'closedir()' to list the
   contents of each of the directories specified in its command-line(or
   in the current working directory if no arguments are supplied). */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <errno.h>
#include <string.h>
#include <dirent.h>

/* List all files in directory 'dirPath' */
static void listFiles(const char *dirpath)
{
   DIR *dirp;
   struct dirent *dp;
   unsigned char isCurrent;      // True if 'dirpath' is '.'

   isCurrent = strcmp(dirpath, ".") == 0;

   dirp = opendir(dirpath);
   assert (dirp != NULL);

   // For each entry in this directory, print directory + filename

   while (1)
   {
      errno = 0;     // To distinguish error from end-of-directory
      dp = readdir(dirp);
      if (dp == NULL)
         break;

      if (!strcmp(dp->d_name, ".") || !strcmp(dp->d_name, ".."))
         continue;      // Skit . and ..

      if (!isCurrent)
         printf("%s/", dirpath);
      printf("%s\n", dp->d_name);
   }
   
   assert (errno == 0);
   assert (closedir(dirp) != -1);
}  

void main(int argc, char *argv[])
{
   assert (argc <= 1 || strcmp(argv[1], "--help"));

   if (argc == 1)          // No arguments - use current directory
      listFiles(".");
   else
      for (argv++; *argv; argv++)
         listFiles(*argv);

   exit(EXIT_SUCCESS);
}
