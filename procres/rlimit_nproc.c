/* Setting the RLIMIT_NPROC resource limit */

#include "../linux.h"

/* Print 'msg' followed by limits for 'resource' */
int printRlimit(const char *msg, int resource)
{
   struct rlimit rlim;

   if (getrlimit(resource, &rlim) == -1)
      return -1;

   printf("%s soft=", msg);
   if (rlim.rlim_cur == RLIM_INFINITY)
      printf("infinite");
#ifdef RLIM_SAVED_CUR
   else if (rlim.rlim_cur == RLIM_SAVED_CUR)
      printf("unrepresentable");
#endif
   else
      printf("%lld", (long long) rlim.rlim_cur);

   printf("; hard=");
   if (rlim.rlim_max == RLIM_INFINITY)
      printf("infinity\n");
#ifdef RLIM_SAVED_MAX
   else if (rlim.rlim_max == RLIM_SAVED_MAX)
      printf("unrepresentable");
#endif
   else
      printf("%lld\n", (long long) rlim.rlim_max);

   return 0;
}

void main(int argc, char *argv[])
{
   struct rlimit rlim;
   int i;
   pid_t childPid;

   if (argc != 3 || strcmp(argv[1], "--help") == 0)
      usage_error("Wrong command-line usage");

   printRlimit("Initial maximum process limits: ", RLIMIT_NPROC);

   /* Set new process limits (hard == soft if no specified) */

   rlim.rlim_cur = (argv[1][0] == 'i') ? RLIM_INFINITY : atoi(argv[1]);
   rlim.rlim_max = (argv[2][0] == 'i') ? RLIM_INFINITY : atoi(argv[2]);

   if (setrlimit(RLIMIT_NPROC, &rlim) == -1)
      syscall_error();

   printRlimit("New maximum process limits: ", RLIMIT_NPROC);

   /* Create as children as possible */

   for (i = 1; ; i ++) {
      switch (childPid = fork()) {
         case -1:
            syscall_error();
         case 0:
            _exit(EXIT_SUCCESS);
         default:
            printf("Child %d (PID=%ld) started\n", i, (long) childPid);
            break;
      }
   }
}
