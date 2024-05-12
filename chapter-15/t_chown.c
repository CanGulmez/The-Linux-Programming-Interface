/* The use of 'chown()' is demonstrated, a program that allows the 
   user to change the owner and group of an arbitrary number of files, 
   specified as command-line arguments. */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <string.h>
#include <assert.h>
#include <pwd.h>
#include <grp.h>

typedef enum {
   FALSE, 
   TRUE, 
} boolean;

/* Return UID corresponding to 'name', or -1 on error */
uid_t userIdFromName(const char *name)
{
   struct passwd *pwd;
   uid_t u;
   char *endptr;

   if (name == NULL || *name == '\0')  // On NULL or empty string
      return -1;                       // return an error

   u = strtol(name, &endptr, 10)       // As a convenience to caller
   if (*endptr == '\0')                // allow a numeric string
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

   if (name == NULL || *name == '\0')  // On NULL or empty string
      return -1;                       // return an error

   g = strtol(name, &endptr, 10)       // As a convenience to caller
   if (*endptr == '\0')                // allow a numeric string
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
   boolean errFnd;

   assert (argc >= 3 && strcmp(argv[1], "--help"));

   if (strcmp(argv[1], "-") == 0)    // "-" ==> don't change owner
      uid = -1;
   else                              // Turn user name into UID
      uid = userIdFromName(argv[1]), 
      assert (uid != -1);

   if (strcmp(argv[2], "-") == 0)    // "-" ==> don't change group
      gid = -1;
   else                              // Turn group name into UID
      gid = groupIdFromName(argv[2]), 
      assert (gid != -1);

   // Change ownership of all files named in remaining arguments

   errFnd = FALSE;
   for (int i=0; i<argc; i++)
      if (chown(argv[i], uid, gid) == -1)
         errFnd = TRUE, 
         abort();

   exit(errFnd ? EXIT_FAILURE : EXIT_SUCCESS);
}
