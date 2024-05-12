/* The program uses the systems calls and library functions described
   in the preceding pages to retrieve all of the process's user and
   group IDs, and then displays them. */

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <limits.h>
#include <sys/fsuid.h>
#include <pwd.h>
#include <grp.h>

#define SG_SIZE      (NGROUPS_MAX + 1)

/* Return name corresponding to 'uid', or NULL on error */
char *userNameFromId(uid_t uid)
{
   struct passwd *pwd;  

   pwd = getpwuid(uid);
   return (pwd == NULL) ? NULL : pwd->pw_name;
}

/* Return name corresponding to 'gid', or NULL on error */
char *groupNameFromId(gid_t gid)
{
   struct group *grp;

   grp = getgrgid(gid);
   return (grp == NULL) ? NULL : grp->gr_name;
}

void main(int argc, char *argv)
{
   uid_t ruid, euid, suid, fsuid;
   gid_t rgid, egid, sgid, fsgid;
   gid_t suppGroups[SG_SIZE];
   int numGroups;
   char *p;

   assert (getresuid(&ruid, &euid, &suid) != -1); 
   assert (getresuid(&rgid, &egid, &sgid) != -1); 

   // Attempts to change the file-system IDs are always ignored for
   // unprivileged processes, but even so, the following calls return
   // the current file-system IDs.

   fsuid = setfsuid(0);
   fsgid = setfsgid(0);

   printf("UID: ");
   p = userNameFromId(ruid);
   printf("real=%s (%ld); ", (p == NULL) ? "???" : p, (long) ruid);
   p = userNameFromId(euid);
   printf("eff=%s (%ld); ", (p == NULL) ? "???" : p, (long) euid);
   p = userNameFromId(suid);
   printf("saved=%s (%ld); ", (p == NULL) ? "???" : p, (long) suid);
   p = userNameFromId(fsuid);
   printf("fs=%s (%ld); ", (p == NULL) ? "???" : p, (long) fsuid);
   printf("\n");

   printf("GID: ");
   p = groupNameFromId(rgid);
   printf("real=%s (%ld); ", (p == NULL) ? "???" : p, (long) rgid);
   p = groupNameFromId(egid);
   printf("eff=%s (%ld); ", (p == NULL) ? "???" : p, (long) egid);
   p = groupNameFromId(sgid);
   printf("saved=%s (%ld); ", (p == NULL) ? "???" : p, (long) sgid);
   p = groupNameFromId(fsgid);
   printf("fs=%s (%ld); ", (p == NULL) ? "???" : p, (long) fsgid);
   printf("\n");
 
   numGroups = getgroups(SG_SIZE, suppGroups);
   assert (numGroups != -1);

   printf("Supplementary groups (%d): ", numGroups);

   for (int i=0; i<numGroups; i++)
   {
      p = groupNameFromId(suppGroups[i]);
      printf("%s (%ld) ", (p == NULL) ? "???" : p, 
             (long) suppGroups[i]);
   }
   printf("\n");

   exit(EXIT_SUCCESS);
}
