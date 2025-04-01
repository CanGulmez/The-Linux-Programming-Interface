/* Scanning a directory */

#include "../linux.h"

/* List all files in directory 'dirPath' */
void listFiles(const char *dirpath)
{
   DIR *dirp;
   struct dirent *dp;
   unsigned char isCurrent;   /* True if 'dirpath' is '.' */

   isCurrent = strcmp(dirpath, ".") == 0;

   dirp = opendir(dirpath);
   if (dirp == NULL) 
      syscall_error();

   /* For each entry in this directory, print directory + filename */

   for (;;) {
      errno = 0;  /* To distinguish error from end-of-directory */
      dp = readdir(dirp);
      if (dp == NULL)
         break;

      if (strcmp(dp->d_name, ".") == 0 || strcmp(dp->d_name, "..") == 0)
         continue;   /* Skip . and .. */

      if (!isCurrent)
         printf("%s/", dirpath);
      printf("%s\n", dp->d_name);
   }

   if (errno != 0)
      syscall_error();

   if (closedir(dirp) == -1)
      syscall_error();
}

void main(int argc, char *argv[])
{
   if (argc > 1 && strcmp(argv[1], "--help") == 0)
      usage_error("Wrong command-line usage");

   if (argc == 1)
      listFiles(".");
   else
      listFiles(argv[1]);

   exit(EXIT_SUCCESS);
}
