/* The program illustrates the use of 'umask()' in conjunction 
   with 'open()' and 'mkdir()'. 
*/

#include "./utils.h"

#define MYFILE        "./newfile2.txt"
#define MYDIR         "/home/can/Downloads/newdir"
#define FILE_PERMS    (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP)
#define DIR_PERMS     (S_IRWXU | S_IRWXG | S_IRWXO)
#define UMASK_SETTING (S_IWGRP | S_IXGRP | S_IWOTH | S_IXOTH)

#define FP_SPECIAL   1  /* Include set-user-ID, set-group-ID, and 
                           sticky bit information in returned string */

#define STR_SIZE     sizeof("rwxrwxrwx")

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
            (((perm & 0b01000) && (flags & FP_SPECIAL)) ? 't' : 'x') :
            (((perm & 0b01000) && (flags & FP_SPECIAL)) ? 'T' : '-'));  

   return str; 
} 
 
void main(int argc, char *argv[])
{
   int fd;
   struct stat sb;
   mode_t mode;

   umask(UMASK_SETTING); 

   fd = open(MYFILE, O_RDWR | O_CREAT | O_EXCL, FILE_PERMS);
   if (fd == -1)
      raiseError("open() did not work", file, line);
   
   if (mkdir(MYDIR, DIR_PERMS) == -1)
      raiseError("mkdir() did not work", file, line);

   mode = umask(0);        /* Retrieves (and clears) umask value */

   if (stat(MYFILE, &sb) == -1)
      raiseError("stat() did not work", file, line);
   
   printf("Requested file perms: %s\n", filePermStr(FILE_PERMS, 0));
   printf("Process:              %s\n", filePermStr(mode, 0));
   printf("Actual file perms:    %s\n\n", filePermStr(sb.st_mode, 0));

   if (stat(MYDIR, &sb) == -1)
      raiseError("stat() did not work", file, line);

   printf("Requested dir. perms: %s\n", filePermStr(DIR_PERMS, 0));
   printf("Process:              %s\n", filePermStr(mode, 0));
   printf("Actual file perms:    %s\n\n", filePermStr(sb.st_mode, 0));

   if (unlink(MYFILE) == -1)
      raiseError("unlink() did not work", file, line);
   if (rmdir(MYDIR) == -1)
      raiseError("rmdir() did not work", file, line);

   exit(EXIT_SUCCESS);
}
 