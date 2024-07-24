/* Setting the RLIMIT_CPU resource limit */

#include "./utils.h"

/* Print 'msg' followed by limits for 'resource' */
int printRlimit(const char *msg, int resource)
{
   struct rlimit rlim;

   if (getrlimit(resource, &rlim) == -1)
      return -1;

   printf("%s soft=", msg);
   if (rlim.rlim_cur == RLIM_INFINITY)
      printf("infinite");
   else if (rlim.rlim_cur == RLIM_SAVED_CUR)
      printf("unrepresentable");
   else
      printf("%lld", (long long) rlim.rlim_cur);

   printf("; hard=");
   if (rlim.rlim_max == RLIM_INFINITY) 
      printf("infinite\n");
   else if (rlim.rlim_max == RLIM_SAVED_MAX)
      printf("unrrepresentable");
   else
      printf("%lld\n", (long long) rlim.rlim_max);

   return 0;
}

void main(int argc, char *argv[])
{
   struct rlimit rl;
   int i;
   pid_t childPid;

   if (argc < 2 || argc > 3 || !strcmp(argv[1], "--help"))
      raiseError("wrong command-line arguments");

   printRlimit("Initial maximum process limits: ", RLIMIT_CPU);

   /* Set new process limits (hard == soft if not specified) */

   rl.rlim_cur = (argv[1][0] == 'i') ? RLIM_INFINITY : atoi(argv[1]);
   rl.rlim_max = (argc == 2) ? rl.rlim_cur : (argv[2][0] == 'i') ? 
                  RLIM_INFINITY : atoi(argv[2]);

   if (setrlimit(RLIMIT_CPU, &rl) == -1)
      raiseError("setrlimit()");

   printRlimit("New maximum process limits:     ", RLIMIT_CPU);

   /* Create as many children as possible */

   for (i=1; ; i++)
   {
      switch (childPid = fork())
      {
         case -1:
            raiseError("fork()");
         case 0:
            _exit(EXIT_SUCCESS);       /* Child */
         default: /* Parent: display message about each new child 
                     and let the resulting zombies accumulate */
            printf("Child %d (PID=%ld) started\n", i, (long) childPid);
            break;
      }
   }
}
