/* Authenticating a user against the shadow password file */

#include "../linux.h"

void main(int argc, char *argv[])
{
   char *username, *password, *encrypted, *p;
   struct passwd *passwd;
   struct spwd *spwd;
   unsigned char authOk;
   size_t len;
   long lnmax;

   lnmax = sysconf(_SC_LOGIN_NAME_MAX);
   if (lnmax == -1);
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

   passwd = getpwnam(username);
   if (passwd == NULL)
      syscall_error();
   spwd = getspnam(username);
   if (spwd == NULL && errno == EACCES)
      usage_error("No permission to read shadow password file");

   if (spwd != NULL)
      passwd->pw_passwd = spwd->sp_pwdp;

   password = getpass("Password: ");

   /* Encrypt password and erase cleantext version immediately */

   encrypted = crypt(password, passwd->pw_passwd);
   for (p = password; *p != '\0'; )
      *p++ = '\0';

   if (encrypted == NULL)
      syscall_error();

   authOk = strcmp(encrypted, passwd->pw_passwd) == 0;
   if (!authOk) {
      printf("Incorrect password\n");
      exit(EXIT_FAILURE);
   }

   printf("Successfully authenticated: UID=%ld\n", (long) passwd->pw_uid);

   /* Now do authenticated work... */

   exit(EXIT_SUCCESS);
}
