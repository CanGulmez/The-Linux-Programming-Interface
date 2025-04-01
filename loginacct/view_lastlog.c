/* Displaying information from the 'lastlog' file */

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

void main(int argc, char *argv[])
{
   struct lastlog llog;
   int fd, i;
   uid_t uid;

   if (argc > 1 && strcmp(argv[1], "--help") == 0)
      usage_error("Wrong command-line usage");

   fd = open(_PATH_LASTLOG, O_RDONLY);
   if (fd == -1)
      syscall_error();

   for (i = 1; i < argc; i++) {
      uid = userIdFromName(argv[i]);
      if (uid == -1) {
         printf("No such user: %s\n", argv[i]);
         continue;
      }

      if (lseek(fd, uid * sizeof(struct lastlog), SEEK_SET) == -1)
         syscall_error();

      if (read(fd, &llog, sizeof(struct lastlog)) <= 0) {
         printf("read failed for %s\n", argv[i]);  /* EOF or error */
         continue;
      }

      printf("%-8.8s %-6.6s %-20.20s %s\n", argv[i], 
         llog.ll_line, llog.ll_host, ctime((time_t *) &llog.ll_time));
   }
   close(fd);

   exit(EXIT_SUCCESS);
}
