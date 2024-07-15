/* The program uses the system calls and library functions to retrieve 
   all of the process's user and group IDs, and then displays them. 
*/

#include "./utils.h"

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

void main(int argc, char *argv[])
{
   uid_t ruid, euid, suid, fsuid;
   gid_t rgid, egid, sgid, fsgid;
   gid_t suppGroups[SG_SIZE];
   int numGroups, i;
   char *p;

   if (getresuid(&ruid, &euid, &suid) == -1)
      raiseError("getresuid() did not work");
   if (getresgid(&rgid, &egid, &sgid) == -1)
      raiseError("getresgid() did not work");

   /* Attempts to change the file-system IDs are always ignored
      for unprivileged processes, but even so, the following
      calls return the current file-system IDs */

   fsuid, fsgid = setfsuid(0), setfsgid(0);

   printf("UID: ");
   p = userNameFromId(ruid);
   printf("real=%s (%ld); ", (p == NULL) ? "???" : p, (long) ruid);
   p = userNameFromId(euid);
   printf("eff=%s (%ld); ", (p == NULL) ? "???" : p, (long) ruid);
   p = userNameFromId(suid);
   printf("saved=%s (%ld); ", (p == NULL) ? "???" : p, (long) ruid);
   p = userNameFromId(fsuid);
   printf("fs=%s (%ld); ", (p == NULL) ? "???" : p, (long) ruid);
   printf("\n");

   printf("GID: ");
   p = groupNameFromId(rgid);
   printf("real=%s (%ld); ", (p == NULL) ? "???" : p, (long) rgid);
   p = groupNameFromId(euid);
   printf("eff=%s (%ld); ", (p == NULL) ? "???" : p, (long) rgid);
   p = groupNameFromId(suid);
   printf("saved=%s (%ld); ", (p == NULL) ? "???" : p, (long) rgid);
   p = groupNameFromId(fsuid);
   printf("fs=%s (%ld); ", (p == NULL) ? "???" : p, (long) rgid);
   printf("\n");

   numGroups = getgroups(SG_SIZE, suppGroups);
   if (numGroups == -1)
      raiseError("getgroups() did not work");

   printf("Supplemantary groups (%d): ", numGroups);
   for (i=0; i<numGroups; i++)
   {
      p = groupNameFromId(suppGroups[i]);
      printf("%s (%ld) ", (p == NULL) ? "???" : p, (long) suppGroups);
   }
   printf("\n");

   exit(EXIT_SUCCESS); 
}
