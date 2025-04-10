/* GLobbing filename patterns with 'popen()' */

#include "../linux.h"

#define POPEN_FMT       "/bin/ls -d %s 2> /dev/null"
#define PAT_SIZE        50
#define PCMD_BUF_SIZE   (sizeof(POPEN_FMT) + PAT_SIZE)

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

void main(int argc, char *argv[])
{
   char pat[PAT_SIZE];           /* Pattern for globbing */
   char popenCmd[PCMD_BUF_SIZE];
   FILE *fp;                     /* File stream returned by popen() */
   unsigned char badPattern;     /* Invalid characters in 'pat'? */
   int len, status, fileCnt, i;
   char pathname[PATH_MAX];

   for (;;) {     /* Read pattern, display results of globbing */
      printf("pattern: ");
      fflush(stdout);
      if (fgets(pat, PAT_SIZE, stdin) == NULL)
         break;                  /* EOF */
      len = strlen(pat);
      if (len <= 1)              /* Empty line */
         continue;

      if (pat[len - 1] == '\n')  /* Strip trailing newline */
         pat[len - 1] = '\0';

      /* Ensure that the pattern contains only valid characters,
         i.e., letters, digits, underscore, dot, and the shell
         globbing characters. (Our defination of valid is more
         resprictive than the shell, which permits other characters
         to be included in a filename if they are quoted.) */

      for (i = 0, badPattern = 0; i < len && !badPattern; i++)
         if (!isalnum((unsigned char) pat[i]) &&
             strchr("_*?[^-].", pat[i]) == NULL)
            badPattern = 1;

      if (badPattern) {
         printf("Bad pattern character: %c\n", pat[i - 1]);
         continue;
      }

      /* Build and execute command to glob 'pat' */

      snprintf(popenCmd, PCMD_BUF_SIZE, POPEN_FMT, pat);
      popenCmd[PCMD_BUF_SIZE - 1] = '\0'; 

      fp = popen(popenCmd, "r");
      if (fp == NULL) {
         printf("popen() failed\n");
         continue;
      }

      /* Read resulting list of pathnames until EOF */

      fileCnt = 0;
      while (fgets(pathname, PATH_MAX, fp) != NULL) {
         printf("%s", pathname);
         fileCnt++;
      }

      /* Close pipe, fetch and display termination status */

      status = pclose(fp);
      printf("\t%d matching file%s\n", fileCnt, (fileCnt != 1) ? "s" : "");
      printf("\tpclose() status == %#x\n", (unsigned int) status);
      if (status != -1)
         printWaitStatus("\t", status);
   }
   
   exit(EXIT_SUCCESS);
}
