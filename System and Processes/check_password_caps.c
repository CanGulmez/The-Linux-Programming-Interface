/* A capability-aware program that authenticates a user 

   This program authenticates a username plus password against the standard
   password database. We noted that the program requires privilege in order
   to read the shadow password file, which is protected (CAP_DAC_READ_SEARCH) 
   to prevent reading by users other than root or members of the shadow group.
*/

#include "./utils.h"

/* Change setting of capability in caller's effective capabilities */

int modifyCap(int capability, int setting)
{
   cap_t caps;
   cap_value_t capList[1];

   /* Retrieve caller's current capabilties */

   caps = cap_get_proc();
   if (caps == NULL)
      raiseError("cap_get_proc()");

   /* Change setting of 'capability' in the effective set of 'caps'. 
      The third argument, 1, is the number of items in the array 
      'capList' */

   capList[0] = capability;
   if (cap_set_flag(caps, CAP_EFFECTIVE, 1, capList, setting) == -1)
   {
      cap_free(caps);
      return -1;
   }

   /* Free the structure that was allocated by libcap */

   if (cap_free(caps) == -1)
      return -1;

   return 0;
}
 
/* Raise capability in caller's effective set */

int raiseCap(int capability)
{
   return modifyCap(capability, CAP_SET);
}

/* An analogous dropCap() (unneeded in this program), could be defined
   as: modifyCap(capability, CAP_CLEAR); */

/* Drop aşş cabalities from all sets */
int dropAllCaps(void)
{
   cap_t empty;
   int s;

   empty = cap_init();
   if (empty == NULL)
      return -1; 

   s = cap_set_proc(empty);

   if (cap_free(empty) == -1)
      return -1;

   return s;
}

void main(int argc, char *argv[])
{
   char *username, *password, *encrypted, *p;
   struct passwd *pwd;
   struct spwd *spwd;
   unsigned char authOk;
   size_t len;
   long lnmax;

   lnmax = sysconf(_SC_LOGIN_NAME_MAX);
   if (lnmax == -1)                 /* If limit is indeterminate */
      lnmax = 256;                  /* make a gues */

   username = malloc(lnmax);
   if (username == NULL)
      raiseError("malloc()");

   printf("Username: ");
   fflush(stdout);
   if (fgets(username, lnmax, stdin) == NULL)
      exit(EXIT_FAILURE);           /* Exit on EOF */

   len = strlen(username);
   if (username[len - 1] == '\n')
      username[len - 1] = '\0';     /* Remove trailing '\n' */

   pwd = getpwnam(username);
   if (pwd == NULL)
      raiseError("getpwnam()");

   /* Only raise CAP_DAC_READ_SEARCH for as long as we need it */

   if (raiseCap(CAP_DAC_READ_SEARCH) == -1)
      raiseError("raiseCap()");

   spwd = getspnam(username);
   if (spwd == NULL && errno == EACCES)
      raiseError("getspnam()");

   /* At this point, we won't need any more capabilities, 
      so frop all capabilities from all sets */

   if (dropAllCaps() == -1)
      raiseError("dropAllCaps()");

   if (spwd != NULL)             /* If there is a shadow password record */
      pwd->pw_passwd = spwd->sp_pwdp; /* Use the shadow password */

   password = getpass("Password: ");

   /* Encypt password and erase cleartext version immediately */

   encrypted = crypt(password, pwd->pw_passwd);
   for (p = password; *p != '\0'; )
      *p++ = '\0';

   if (encrypted == NULL)
      raiseError("crypt()");

   authOk = strcmp(encrypted, pwd->pw_passwd) == 0;
   if (!authOk)
   {
      printf("Incorrent password\n");
      exit(EXIT_FAILURE);
   }

   printf("Successfully authenticated: UID=%ld\n", (long) pwd->pw_uid);

   /* Now do authenticated work... */

   exit(EXIT_SUCCESS);
}
