/* Retrieving and interpreting file 'stat' information */

#include "../linux.h"

#define FP_SPECIAL      1
#define STR_SIZE        sizeof("rwxrwxrwx")

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
         (((perm & S_ISVTX) && (flags & FP_SPECIAL)) ? 'T' : '-'));

   return str;
}

void displayStatInfo(const struct stat *sb)
{
   printf("File type: ");

   switch (sb->st_mode & S_IFMT) {
      case S_IFREG:  printf("regular file\n");           break;
      case S_IFDIR:  printf("directory\n");              break;
      case S_IFCHR:  printf("character device\n");       break;
      case S_IFBLK:  printf("block device\n");           break;
      case S_IFLNK:  printf("symbolic (soft) link\n");   break;
      case S_IFIFO:  printf("FIFO or pipe\n");           break;
      case S_IFSOCK: printf("socket\n");                 break;
      default:       printf("unknown file type?\n");
   }
   
   // printf("Device containing i-node: major=%ld, minor=%ld\n",
   //        (long) major(sb->st_dev), (long) minor(sb->st_dev));
   printf("I-node number: %ld\n", (long) sb->st_ino);
   printf("Mode: %lo (%s)\n", (unsigned long) sb->st_mode, filePermStr(sb->st_mode, 0));

   if (sb->st_mode & (S_ISUID | S_ISGID | S_ISVTX))
      printf("special bits set: %s%s%s\n",
         (sb->st_mode & S_ISUID) ? "set-UID " : "",
         (sb->st_mode & S_ISGID) ? "set-GID " : "",
         (sb->st_mode & S_ISVTX) ? "sticky " : "");
   
   printf("Number of (hard) links: %ld\n", (long) sb->st_nlink);
   printf("Ownership: UID=%ld GID=%ld\n", (long) sb->st_uid, (long) sb->st_gid);

   // if (S_ISCHR(sb->st_mode) || S_ISBLK(sb->st_mode))
   //    printf("Device number (st_rdev): major=%ld; minor=%ld\n",
   //       (long) major(sb->st_rdev), (long) minor(sb->st_rdev));

   printf("File size: %lld bytes\n", (long long) sb->st_size);
   printf("Optimal I/O block size: %ld bytes\n", (long) sb->st_blksize);
   printf("512B blocks allocated: %lld\n", (long long) sb->st_blocks);
   printf("Last file access: %s", ctime(&sb->st_atim.tv_sec));
   printf("Last file modification: %s", ctime(&sb->st_mtim.tv_sec));
   printf("Last status change: %s", ctime(&sb->st_ctim.tv_sec));
}

void main(int argc, char *argv[])
{
   struct stat sb;
   unsigned char statLink;
   int fname;

   statLink = (argc > 1) && strcmp(argv[1], "-l") == 0;
   fname = statLink ? 2 : 1;

   if (fname >= argc || (argc > 1 && strcmp(argv[1], "--help") == 0))
      syscall_error();

   if (statLink) {
      if (lstat(argv[fname], &sb) == -1)
         syscall_error();
   } else {
      if (stat(argv[fname], &sb) == -1)
         syscall_error();
   }

   displayStatInfo(&sb);

   exit(EXIT_SUCCESS);
}
