/* The program uses 'stat()' to retrieve information about the file named
   on its command line. If the '-l' command-line option is specified, then
   the program instead uses 'lstat()' so that we can retrieve information
   about a symbolic link instead of the file to which it refers. 
*/

#include "./utils.h"

void displayStatInfo(const struct stat *sb)
{
   printf("File type:               ");
   switch (sb->st_mode & S_IFMT) 
   {
      case S_IFREG:  printf("regular file\n");           break;
      case S_IFDIR:  printf("directory\n");              break;
      case S_IFCHR:  printf("character device\n");       break;
      case S_IFBLK:  printf("block device\n");           break;
      case S_IFLNK:  printf("symbolic (soft) link\n");   break;
      case S_IFIFO:  printf("FIFO or pipe\n");           break;
      case S_IFSOCK: printf("socket\n");                 break;
      default:       printf("unknown file type?\n");     break;
   }
   printf("I-node number:           %ld\n", (long) sb->st_ino);
   printf("Mode:                    %lo\n", (unsigned long) sb->st_mode);

   if (sb->st_mode & (S_ISUID | S_ISGID | S_ISVTX)) 
      printf("    special bits set:    %s%s%s\n", 
             (sb->st_mode & S_ISUID) ? "set-UID" : "", 
             (sb->st_mode & S_ISGID) ? "set-GID" : "", 
             (sb->st_mode & S_ISVTX) ? "sticky" : "");
   
   printf("Number of (hard) links:  %ld\n", (long) sb->st_nlink);

   printf("Ownership:               UID=%ld   GID=%ld\n", 
          (long) sb->st_uid, (long) sb->st_gid);

   printf("File size:               %lld bytes\n", (long long) sb->st_size);
   printf("Optimal I/O block size:  %ld bytes\n", (long) sb->st_blksize);
   printf("512B blocks allocated:   %lld\n", (long long) sb->st_blocks);
   printf("Last file access:        %s", ctime(&sb->st_atim.tv_sec));
   printf("Last file modification:  %s", ctime(&sb->st_mtim.tv_sec));
   printf("Last status change:      %s", ctime(&sb->st_ctim.tv_sec));
}

void main(int argc, char *argv[])
{
   struct stat sb; 
   unsigned char statLink;   // True if "-l" specified (i.e., use lstat)
   int fname;                // Location of filename argument in argv[]

   statLink = (argc > 1) && !strcmp(argv[1], "-l"); /* Simple parsing for "-l" */

   fname = statLink ? 2 : 1;

   if (fname >= argc || (argc > 1 && !strcmp(argv[1], "--help")))
      raiseError("use lstat() instead of stat()", file, line);
   
   if (statLink)
   {
      if (lstat(argv[fname], &sb) == -1)
         raiseError("lstat() did not work", file, line);
   }
   else
   {
      if (stat(argv[fname], &sb) == -1)
         raiseError("stat() did not work", file, line);
   }

   displayStatInfo(&sb);   

   exit(EXIT_SUCCESS);
}
 