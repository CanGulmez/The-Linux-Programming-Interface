/* The program illustrates the use of 'umask()' in conjunction with 
   'open()' and 'mkdir()'. */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <fcntl.h>
#include <sys/stat.h>

#define MYFILE        "myfile"
#define MYDIR         "mydir"
#define FILE_PERMS    (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP)
#define DIR_PERMS     (S_IRWXU | S_IRWXG | S_IRWXU)
#define UMASK_SETTING (S_IWGRP | S_IXGRP | S_IWOTH | S_IXOTH)

#define FP_SPECIAL   1  /* Include set-user-ID, set-grpoup-ID, and
                           sticky bit information in returned string */

#define STR_SIZE     sizeof ("rwxrwxtrwx")

/* Return ls(1)-style string for file permissions mask */
char *filePermStr(mode_t perm, int flags) 
{
   static char str[STR_SIZE]; 
   
   snprintf(str, STR_SIZE, "%c%c%c%c%c%c%c%c%c",
      (perm & S_IRUSR) ? 'r' : '-', (perm & S_IWUSR) ? 'w' : '-',
      (perm & S_IXUSR) ?
      (((perm & S_ISUID) && (flags & FP_SPECIAL)) ? 's' : 'x') :
      (((perm & S_ISUID) && (flags & FP_SPECIAL)) ? 'S' : '-'),
      (perm & S_IRGRP) ? 'r' : '-', (perm & S_IWGRP) ? 'w' : '-',
      (perm & S_IXGRP) ?
      (((perm & S_ISGID) && (flags & FP_SPECIAL)) ? 's' : 'x') :
      (((perm & S_ISGID) && (flags & FP_SPECIAL)) ? 'S' : '-'),
      (perm & S_IROTH) ? 'r' : '-', (perm & S_IWOTH) ? 'w' : '-',
      (perm & S_IXOTH) ?
      (((perm & S_ISVTX) && (flags & FP_SPECIAL)) ? 't' : 'x') :
      (((perm & S_ISVTX) && (flags & FP_SPECIAL)) ? 'T' : '-')
   );
   return str; 
}


void main(int argc, char *argv[])
{
   int fd;
   struct stat sb;
   mode_t u;

   umask(UMASK_SETTING);

   fd = open(MYFILE, O_RDWR | O_CREAT | O_EXCL, FILE_PERMS);
   assert (fd != -1);

   assert (mkdir(MYDIR, DIR_PERMS) != -1);

   u = umask(0);        // Retrieve (and clears) umask value

   assert (stat(MYFILE, &sb) != -1);

   printf("Requested file perms: %s\n", filePermStr(FILE_PERMS, 0));
   printf("Process umask:        %s\n", filePermStr(u, 0));
   printf("Actual file perms:    %s\n\n", filePermStr(sb.st_mode, 0));

   assert (stat(MYDIR, &sb) != -1);

   printf("Requested dir. perms: %s\n", filePermStr(DIR_PERMS, 0));
   printf("Process umask:        %s\n", filePermStr(u, 0));
   printf("Actual dir. perms:    %s\n", filePermStr(sb.st_mode, 0));

   assert (unlink(MYFILE) != -1);
   assert (rmdir(MYDIR) != -1);

   exit(EXIT_SUCCESS);
}
