/* Changing the owner and group of a file */

#include "../linux.h"

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

void main(int argc, char *argv[])
{
   uid_t uid;
   gid_t gid;
   int i;
   unsigned char errFnd;

   if (argc < 3 || strcmp(argv[1], "--help") == 0)
      usage_error("Wrong command-line usage");

   if (strcmp(argv[1], "-") == 0) {    /* don't change owner */
      uid = -1;
   } else {
      uid = userIdFromName(argv[1]);
      if (uid == -1)
         syscall_error();
   }

   if (strcmp(argv[2], "-") == 0) {    /* don't change group */
      gid = -1;
   } else {
      gid = groupIdFromName(argv[2]);
      if (gid == -1)
         syscall_error();
   }

   /* Change the ownership of all files named in remaining arguments */

   errFnd = 0;
   for (i = 3; i < argc; i++) {
      if (chown(argv[i], uid, gid) == -1)
         syscall_error();
         errFnd = 1;
   }

   exit(errFnd ? EXIT_FAILURE : EXIT_SUCCESS);
}
