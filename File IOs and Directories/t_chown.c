/* The program that allows the user to change the owner and group of
   arbitrary number of files, specified as command-line arguments. 
*/

#include "./utils.h"

/* Return UID corresponding to 'name', or -1 on error */
uid_t userIdFromName(const char *name)
{
   struct passwd *pwd; 
   uid_t u;
   char *endptr;

   if (name == NULL || *name == '\0')  /* On NULL or empty string */
      return -1;                       /* return an error */

   u = strtol(name, &endptr, 10);      /* As a convenience to caller */
   if (*endptr == '\0')                /* allow a numeric string */
      return u;

   pwd = getpwnam(name);
   if (pwd == NULL)
      return -1;

   return pwd->pw_uid;
}

/* Return GID corresponding to 'name', or -1 on error */
gid_t groupIdFromName(const char *name)
{
   struct group *grp; 
   gid_t g;
   char *endptr;

   if (name == NULL || *name == '\0')  /* On NULL or empty string */
      return -1;                       /* return an error */

   g = strtol(name, &endptr, 10);      /* As a convenience to caller */
   if (*endptr == '\0')                /* allow a numeric string */
      return g;

   grp = getgrnam(name);
   if (grp == NULL)
      return -1;

   return grp->gr_gid;
}

void main(int argc, char *argv[])
{
   uid_t uid;
   gid_t gid;
   int i;
   unsigned char errFnd;

   if (argc < 3 || !strcmp(argv[1], "--help"))
      raiseError("wrong command-line arguments", file, line);

   if (!strcmp(argv[1], "-"))    /* "-" ==> don't change owner */
      uid = -1;
   else 
   {
      uid = userIdFromName(argv[1]);
      if (uid == -1)
         raiseError("userIdFromName() did not work", file, line);
   }

   if (!strcmp(argv[2], "-"))    /* "-" ==> don't change group */
      gid = -1;
   else
   {
      gid = groupIdFromName(argv[2]);
      if (gid == -1)
         raiseError("groupIdFromName() did not work", file, line);
   }

   /* Change ownership of all files named in remaning arguments */

   errFnd = 0;
   for (i=3; i<argc; i++)
   {
      if (chown(argv[i], uid, gid) == -1)
      {
         raiseError("chown() did not work", file, line);
         errFnd = 1;
      }
   }

   exit(errFnd ? EXIT_FAILURE : EXIT_SUCCESS);
}
