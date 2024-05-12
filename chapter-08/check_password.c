/* This program demonstrates how to use 'crypt()' to authenticate
   a user. Program first reads a username and then retrieves the 
   corresnponding password record and (if it exist) shadow password
   record. The program prints an error message and exist if no
   password record is found, or if the program doesn't have 
   permission to read from the shadow password file (thid requires
   either superuser privilege or membership) of the shodow group).
   The program then reads the user's password, using the 'getpass()'
   function.*/

#define _DEFAULT_SOURCE
#define _XOPEN_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <limits.h>
#include <string.h>
#include <errno.h>
#include <assert.h>
#include <pwd.h>
#include <shadow.h>

void main(int argc, char *argv) 
{
   char *username, *password, *encrypted, *p;
   struct passwd *pwd;
   struct spwd *spwd;
   unsigned int authOk;
   ssize_t len;
   long lnmax;

   lnmax = sysconf(_SC_LOGIN_NAME_MAX);
   if (lnmax == -1)              // If limit is indeterminate
      lnmax = 256;               // make a guess

   username = malloc(lnmax);
   assert (username != NULL);

   printf("Username: ");
   fflush(stdout);
   assert (fgets(username, lnmax, stdin) != NULL); // Exit on EOF

   len = strlen(username);
   if (username[len - 1] == '\n')
      username[len - 1] = '\0';     // Remove trailing '\n'

   pwd = getpwnam(username);
   assert (pwd != NULL);

   spwd = getspnam(username);
   assert (spwd != NULL && errno != EACCES);

   if (spwd != NULL)       // If there is a shadow password record
      pwd->pw_passwd = spwd->sp_pwdp;   // Use the shadow password

   password = getpass("Password: ");

   // Encrypt password and erase cleartext version immediately

   encrypted = crypt(password, pwd->pw_passwd);
   for (p = password; *p != '\0'; )
      *p++ = '\0';

   assert (encrypted != NULL);

   authOk = strcmp(encrypted, pwd->pw_passwd) == 0;
   if (!authOk)
   {
      printf("Incorrent password\n");
      exit(EXIT_FAILURE);
   }

   printf("Successfully authenticated: UID=%ld\n", 
          (long) pwd->pw_uid);

   // Now do authenticated work...

   exit(EXIT_SUCCESS); 
}
