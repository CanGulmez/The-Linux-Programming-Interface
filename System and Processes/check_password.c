/* Authenticating a user against the shadow password file */

#include "./utils.h"

void main(int argc, char *argv[])
{
   char *username, *password, *encrypted, *p;
   struct passwd *pwd;
   struct spwd *spwd;
   unsigned int authOk;
   size_t len;
   long lnmax;

   lnmax = sysconf(_SC_LOGIN_NAME_MAX);
   if (lnmax == -1)                    /* If limit is indeterminate */
      lnmax = 256;                     /* make a guess */

   username = malloc(lnmax);
   if (username == NULL)
      raiseError("malloc() did not work");

   printf("Username: ");
   fflush(stdout);
   if (fgets(username, lnmax, stdin) == NULL)
      raiseError("fgets() did not work");    /* Exit on EOF */

   len = strlen(username);
   if (username[len - 1] == '\n')
      username[len - 1] = '\0';              /* Remove trailing '\n' */

   pwd = getpwnam(username);
   if (pwd == NULL)
      raiseError("getpwnam() did not work");
   spwd = getspnam(username);
   if (spwd == NULL && errno == EACCES)
      raiseError("getspnam() did not work");

   if (spwd != NULL)         /* If there is a shadow password record */
      pwd->pw_passwd = spwd->sp_pwdp;  /* Use the shadow password */

   password = getpass("Password: ");

   /* Encypt password and erase cleartext version immediately */

   encrypted = crypt(password, pwd->pw_passwd);
   for (p = password; *p != '\0'; )
      *p++ = '\0';

   if (encrypted == NULL)
      raiseError("crypt() did not work");

   authOk = !strcmp(encrypted, pwd->pw_passwd);
   if (!authOk)
   {
      printf("Incorrent password\n");
      exit(EXIT_FAILURE); 
   }

   printf("Successfully authenticated: UID=%ld\n", (long) pwd->pw_uid);

   /* Now do authenticated work... */

   exit(EXIT_SUCCESS);
}
