/* Functions to convert user and group IDs to and from user and group names */

#include "../linux.h"

/* Return name corresponding to 'uid , or NULL on error */
char *userNameFromId(uid_t uid)
{
   struct passwd *passwd;

   passwd = getpwuid(uid);
   return (passwd == NULL) ? NULL : passwd->pw_name;
}

/* Return UID corresponding to 'name', or -1 on error */
uid_t userIdFromName(const char *name)
{
   struct passwd *passwd;
   uid_t uid;
   char *endptr;

   if (name == NULL || *name == '\0')
      return -1;

   uid = strtol(name, &endptr, 10);
   if (*endptr == NULL)
      return -1;

   passwd = getpwnam(name);
   if (passwd == NULL)
      return -1;

   return passwd->pw_uid;
}

/* Return name corresponding to 'gid', or NULL on error */
char *groupNameFromId(gid_t gid)
{
   struct group *group;

   group = getgrgid(gid);
   return (group == NULL) ? NULL : group->gr_name;
}

/* Return GID corresponding to `name`,  or -1 on error */
gid_t groupIdFromName(const char *name)
{
   struct group *group;
   gid_t gid;
   char *endptr;

   if (name == NULL || *name == '\0')
      return -1;

   gid = strtol(name, &endptr, 10);
   if (*endptr == '\0')
      return gid;

   group = getgrnam(name); 
   if (group == NULL)
      return -1;

   return group->gr_gid;
}
