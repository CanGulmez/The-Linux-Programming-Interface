/* Updating the 'utmp' and 'wtmp' files */

#include "../linux.h"

void main(int argc, char *argv[])
{
   struct utmpx ut;
   char *devname;

   if (argc < 2 || strcmp(argv[1], "--help") == 0)
      usage_error("Wrong command-line usage");

   memset(&ut, 0, sizeof(struct utmpx));
   ut.ut_type = USER_PROCESS;          /* This is a user login */
   strncpy(ut.ut_user, argv[1], sizeof(ut.ut_user));
   if (time((time_t *) &ut.ut_tv.tv_sec) == -1)
      syscall_error();
   ut.ut_pid = getpid();

   /* Set ut_line and ut_id based on the terminal associated with
      'stdin'. This code assumes terminals named "/dev/[pt]t[sy]*".
      The "/dev/" dirname is 5 characters; the "[pt]t[sy]" filename
      prefix is 3 characters (making 8 characters in all). */

   devname = ttyname(STDIN_FILENO);
   if (devname == NULL)
      syscall_error();
   if (strlen(devname) <= 8)        /* Should rever happen */
      syscall_error();

   strncpy(ut.ut_line, devname + 5, sizeof(ut.ut_line));
   strncpy(ut.ut_id, devname + 8, sizeof(ut.ut_id));

   printf("Creating login entries in utmp and wtmp\n");
   printf("     using pid %ld, line %.*s, id %.*s\n",
      (long) ut.ut_pid, (int) sizeof(ut.ut_line), ut.ut_line,
      (int) sizeof(ut.ut_id), ut.ut_id);

   setutxent();                     /* Rewind to start of utmp file */
   if (pututxline(&ut) == NULL)     /* Write login record to utmp */
      syscall_error();
   updwtmpx(_PATH_WTMP, &ut);       /* Append login record to wtmp */

   /* Sleep a while, so we can exemine utmp and wtmp files */

   sleep((argc > 2) ? atoi(argv[2]) : 15);

   /* Now do a "logout"; use values from previously initialized 'ut',
      except for changes below */

   ut.ut_type = DEAD_PROCESS;       /* This is a user logout */
   time((time_t *) &ut.ut_tv.tv_sec);
   memset(&ut.ut_user, 0, sizeof(ut.ut_user));
   printf("Creating logout entries in utmp and wtmp\n");
   setutxent();                     /* Rewind to start of 'utmp' file */
   if (pututxline(&ut) == NULL)     /* Overwrite previous utmp record */
      syscall_error();
   updwtmpx(_PATH_WTMP, &ut);       /* Append logout record to wtmp */

   endutxent();
   exit(EXIT_SUCCESS);
}
