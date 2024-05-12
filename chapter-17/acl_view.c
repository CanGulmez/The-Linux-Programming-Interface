/* The program demonstrates the use of some of the ACL library functions.
   This program retrieves and displays the ACL on a file (i.e. it provides 
   a subset of the functionaltiy of the 'getfacl' command). If the '-d'
   commandline option is specified, then the program displays the default
   ACL (of a directory) instead of the access ACL. */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <sys/acl.h>
#include <acl/libacl.h>
#include <pwd.h>
#include <grp.h>

static void usageError(char *progName)
{
   fprintf(stderr, "Usage: %s [-d] filename\n", progName);
   exit(EXIT_FAILURE);
}

char *groupNameFromId(gid_t gid)
{
   struct group *grp;

   grp = getgrgid(gid);
   return (grp == NULL) ? NULL : grp->gr_name;
}

void main(int argc, char *argv[])
{
   acl_t acl;
   acl_type_t type;
   acl_entry_t entry;
   acl_tag_t tag;
   uid_t *uidp;
   gid_t *gidp;
   acl_permset_t permset;
   char *name;
   int entryId, permVal, opt;

   type = ACL_TYPE_ACCESS;
   while ((opt = getopt(argc, argv, "d")) != -1)
   {
      switch (opt)
      {
         case 'd': type = ACL_TYPE_DEFAULT;     break;
         case '?': usageError(argv[0]);
      }
   }

   if (optind + 1 != argc)
      usageError(argv[0]);

   acl = acl_get_file(argv[optind], type);
   assert (acl != NULL);

   // Walk through each entry in this ACL.

   for (entryId=ACL_FIRST_ENTRY; ; entryId = ACL_NEXT_ENTRY)
   {
      if (acl_get_entry(acl, entryId, &entry) != 1)
         break;            // Exit on error or no more entries

      // Retrieve and display tag type.

      assert (acl_get_tag_type(entry, &tag) != -1);

      printf("%-12s", (tag == ACL_USER_OBJ)  ? "user_obj"  :
                      (tag == ACL_USER)      ? "user"      :
                      (tag == ACL_GROUP_OBJ) ? "group_obj" :
                      (tag == ACL_GROUP)     ? "group"     :
                      (tag == ACL_MASK)      ? "mask"      :
                      (tag == ACL_OTHER)     ? "other"     : "???");

      // Retrieve and display optional tag qualifier.

      if (tag == ACL_USER)
      {
         uidp = acl_get_qualifier(entry);
         assert (uidp != NULL);

         name = groupNameFromId(*uidp);
         if (name == NULL)
            printf("%-8d ", *uidp);
         else
            printf("%-8s ", name);

         assert (acl_free(uidp) != -1);
      } 
      else if (tag == ACL_GROUP) 
      {
         gidp = acl_get_qualifier(entry);
         assert (gidp != NULL);

         name = groupNameFromId(*gidp);
         if (name == NULL)
            printf("%-8d ", *gidp);
         else
            printf("%-8s ", name);

         assert (acl_free(gidp) != -1)
      }
      else
         printf("       ");

      // Retrieve and display permissions.

      assert (acl_get_permset(entry, &permset) != -1);

      permVal = acl_get_perm(permset, ACL_READ);
      assert (permVal != -1);

      printf("%c", (permVal == 1) ? 'r' : '-');

      permVal = acl_get_perm(permset, ACL_WRITE);
      assert (permVal != -1);

      printf("%c", (permVal == 1) ? 'w' : '-');

      permVal = acl_get_perm(permset, ACL_EXECUTE);
      assert (permVal != -1);

      printf("%c", (permVal == 1) ? 'x' : '-');

      printf("\n");
   }
   assert (acl_free(acl) != -1);

   exit(EXIT_SUCCESS);
}
