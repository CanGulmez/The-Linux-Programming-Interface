/* Reaping dead children via a handler for SIGCHLD */

#include "../linux.h"

#define BUF_SIZE  100

static volatile int numLiveChildren = 0; 
                  /* Number of children started but not yet waited on */

char *currTime(const char *format)
{
   static char buffer[BUF_SIZE];
   time_t t;
   size_t s;
   struct tm *tm;

   t = time(NULL);
   tm = localtime(&t);
   if (tm == NULL)
      return NULL;

   s = strftime(buffer, BUF_SIZE, (format != NULL) ? format : "%c", tm);
   
   return (s == 0) ? NULL : buffer;
}

void printWaitStatus(const char *msg, int status)
{
   if (msg != NULL)
      printf("%s", msg);

   if (WIFEXITED(status)) {
      printf("child exited, status=%d\n", WEXITSTATUS(status));
   } else if (WIFSIGNALED(status)) {
      printf("child killed by signal %d (%s)", 
         WTERMSIG(status), strsignal(WTERMSIG(status)));
#ifdef WCOREDUMP
   if (WCOREDUMP(status))
      printf(" (core dumped)");
#endif
      printf("\n");
   } else if (WIFSTOPPED(status)) {
      printf("child stopped by signal %d (%s)\n",
         WSTOPSIG(status), strsignal(WSTOPSIG(status)));
#ifdef WIFCONTINUED
   } else if (WIFCONTINUED(status)) {
      printf("child continued\n");
#endif
   } else {
      printf("what happened to this child? (status=%x)\n",
         (unsigned int) status);
   }
}

static void sigchldHandler(int sig)
{
   int status, savedErrno;
   pid_t childPid;

   /* UNSAFE: This handler uses non-async-signal-safe functions
      (printf(), printWaitStatus(), currTime(); see Section 21.1.2) */

      savedErrno = errno;        /* In case we modify 'errno' */

   printf("%s handler: Caught SIGCHLD\n", currTime("%T"));

   while ((childPid = waitpid(-1, &status, WNOHANG)) > 0) {
      printf("%s handler: Reaped child %ld - ", currTime("%T"), 
         (long) childPid);
      printWaitStatus(NULL, status);
      numLiveChildren--;
   }

   if (childPid == -1 && errno != ECHILD)
      syscall_error();

   sleep(5);         /* Artificially lengthen execution of handler */
   printf("%s handler: returning\n", currTime("%T"));

   errno = savedErrno;
}

void main(int argc, char *argv[])
{
   int j, sigCnt;
   sigset_t blockMask, emptyMask;
   struct sigaction sa;

   if (argc < 2 || strcmp(argv[1], "--help") == 0)
   usage_error("Wrong command-line usage");

   setbuf(stdout, NULL);      /* Disable buffering of stdout */

   sigCnt = 0;
   numLiveChildren = argc - 1;

   sigemptyset(&sa.sa_mask);
   sa.sa_flags = 0;
   sa.sa_handler = sigchldHandler;
   if (sigaction(SIGCHLD, &sa, NULL) == -1)
      syscall_error();

   /* Block SIGCHLD to prevent its delivery if a child terminates
      before the parent commences the sigsuspend() loop below */

   sigemptyset(&blockMask);
   sigaddset(&blockMask, SIGCHLD);
   if (sigprocmask(SIG_SETMASK, &blockMask, NULL) == -1)
      syscall_error();

   for (j = 1; j < argc; j++) {
      switch (fork()) {
         case -1:
            syscall_error();
         case 0:           /* Child - sleeps and then exits */
            sleep(atoi(argv[j]));
            printf("%s Child %d (PID=%ld) exiting\n", currTime("%T"),
               j, (long) getpid());
            _exit(EXIT_SUCCESS);
         default:          /* Parent - loops to create next child */
            break;
      }
   }

   /* Parent comes here: wait for SIGCHLD until all children are dead */
   
   sigemptyset(&emptyMask);
   while (numLiveChildren > 0) {
      if (sigsuspend(&emptyMask) == -1 && errno != EINTR)
         syscall_error();
      sigCnt++;
   }
   printf("%s All %d children have terminated; SIGCHLD was caught "
      "%d times\n", currTime("%T"), argc - 1, sigCnt);
   
      exit(EXIT_SUCCESS);
}
