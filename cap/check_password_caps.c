/* A capability-aware program that authenticates a user */

#include "../linux.h"

/* Change setting of capability in caller's effective capabilities */
static int modifyCap(int capability, int setting)
{
   cap_t caps;
   cap_value_t capList[1];

   /* Retrieve caller's current capabilities */

   caps = cap_get_proc();
   if (caps == NULL)
      return -1;

   /* Change setting of 'capability' in the effective set of 'caps'. The
      third argument, 1, is the number of times in the array 'capList' */

   capList[0] = capability;
   if (cap_set_flag(caps, CAP_EFFECTIVE, 1, capList, setting) == -1) {
      cap_free(caps);
      return -1;
   }

   /* Push modified capability sets back to kernel, to change
      caller's capabilities */

   if (cap_set_proc(caps) == -1) {
      cap_free(caps);
      return -1;
   }

   /* Free the structure that was allocated by libcap */

   if (cap_free(caps) == -1)
      return -1;

   return 0;
}

/* Raise capability in caller's effective set */
static int raiseCap(int capability)
{
   return modifyCap(capability, CAP_SET);
}

/* Drop all capabilities from all sets */
static int dropAllCaps(void)
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
   if (lnmax == -1)
      lnmax = 256;

   username = malloc(lnmax);
   if (username == NULL)
      syscall_error();

   printf("Username: ");
   fflush(stdout);
   if (fgets(username, lnmax, stdin) == NULL)
      exit(EXIT_FAILURE);

   len = strlen(username);
   if (username[len - 1] == '\n')
      username[len - 1] = '\0';

   pwd = getpwnam(username);
   if (pwd == NULL)
      syscall_error();

   /* Only raise CAP_DAC_READ_SEARCH for as long as we need it */

   if (raiseCap(CAP_DAC_READ_SEARCH) == -1)
      syscall_error();

   spwd = getspnam(username);
   if (spwd == NULL && errno == EACCES)
      syscall_error();

   /* At this point, we won't need any more capabilities,
      so drop all capabilities from all sets */
   
   if (dropAllCaps() == -1)
      syscall_error();

   if (spwd != NULL)
      pwd->pw_passwd = spwd->sp_pwdp;

   password = getpass("Password: ");

   /* Encrypt password and erase cleartext version immediately */
   
   encrypted = crypt(password, pwd->pw_passwd);
   for (p = password; *p != '\0'; )
      *p++ = '\0';

   if (encrypted == NULL)
      syscall_error();

   authOk = strcmp(encrypted, pwd->pw_passwd) == 0;
   if (!authOk) {
      printf("Incorrect password\n");
      exit(EXIT_FAILURE);
   }

   printf("Successfully authenticated: UID=%ld\n", (long) pwd->pw_uid);
   
   /* Now do authenticated work... */
   
   exit(EXIT_SUCCESS);
}
