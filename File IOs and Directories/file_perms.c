/* filePermStr(), which, given a file permissions mask, returns a
   statically allocated string representation of that mask in the 
   same style as is used by 'ls()'. 
*/

#include "../utils.h"

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
