/* The program 'opendir()', 'readdir()', and 'closedir()' to list
   the contents of each of the directories specified in its command
   line (or in the current working directory if no arguments are
   supplied) 
*/

#include "../utils.h"

/* List all files in directory 'dirPath' */
void listFiles(char *dirpath)  
{
   DIR *dirp;
   struct dirent *dp;
   unsigned char isCurrent;         /* True if 'dirpath' is '.' */

   isCurrent = strcmp(dirpath, ".") == 0;

   dirp = opendir(dirpath);
   if (dirp == NULL)
   {
      raiseError("opendir() did not work", file, line);
      return;
   }

   /* For each entry in this directory, print directory + filename */

   for (;;)
   {
      errno = 0;     /* To distinguish error from end-of-directory */
      dp = readdir(dirp);
      if (dirpath == NULL)
         break;

      if (!strcmp(dp->d_name, ".") || !strcmp(dp->d_name, ".."))
         continue;   /* Skit . and .. */

      if (!isCurrent)
         printf("%s/", dirpath);
      printf("%s\n", dp->d_name);
   }

   if (errno != 0)
      raiseError("readdir() did not work", file, line);
   if (closedir(dirp) == -1)
      raiseError("closedir() did not work", file, line);
}

void main(int argc, char *argv[])
{
   if (argc > 1 && !strcmp(argv[1], "--help"))
      raiseError("wrong command-line arguments", file, line);

   if (argc == 1)          /* No arguments - use current directory */
      listFiles(".");
   else
      for (int i=0; i<argc; i++)
         listFiles(argv[i]);

   exit(EXIT_SUCCESS);
}
