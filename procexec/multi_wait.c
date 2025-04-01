/* Creating and waiting for multiple children */

#include "../linux.h"

#define BUF_SIZE  1000

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

void main(int argc, char *argv[])
{
   int numDead;         /* Number of children so far waited for */
   pid_t childPid;      /* PID of child */   
   int i;

   if (argc < 2 || strcmp(argv[1], "--help") == 0)
      syscall_error();

   setbuf(stdout, NULL); /* Disable buffering of stdout */

   for (i = 1; i < argc; i++) {  /* Create one child for each argument */
      switch (fork()) {
         case -1:
            syscall_error();
         case 0:  /* Child */
            printf("%s Child %d started with PID %ld\n", currTime("%T"), i, (long) getpid());
            sleep(atoi(argv[i]));  /* Simulate execution */
            _exit(EXIT_SUCCESS);
         default: /* Parent */
            break;
      }
   }
   numDead = 0;
   for (;;) {     /* Parent waits for each child to exit */
      childPid = wait(NULL);
      if (childPid == -1) {
         if (errno == ECHILD) {
            printf("No more children - bye!\n");
            exit(EXIT_SUCCESS);
         } else {
            syscall_error();
         }
      }
      numDead ++;
      printf("[%s] wait() returned child PID %ld (numDead=%d)\n",
         currTime("%T"), (long) childPid, numDead);
   }
}
