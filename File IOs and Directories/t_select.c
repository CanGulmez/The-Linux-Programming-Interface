/* Using 'select()' to monitor multiple file desccriptors */

#include "./utils.h"

void usageError(const char *progName)
{
   fprintf(stderr, "Usage: %s {timeout|-} fd-num[rw]...\n", progName);
   fprintf(stderr, "    - means infinite timeout;\n");
   fprintf(stderr, "    r = monitor for read\n");
   fprintf(stderr, "    w = monitor for write\n\n");
   fprintf(stderr, "    e.g.: %s - rw 1w\n", progName);
   exit(EXIT_FAILURE);
}

void main(int argc, char *argv[])
{
   fd_set readfds, writefds;
   int ready, nfds, fd, numRead, i;
   struct timeval timeout;
   struct timeval *pto;
   char buffer[10];                 /* Large enough to hold "rw\0" */

   if (argc < 2 || !strcmp(argv[1], "--help"))
      usageError(argv[0]);

   /* Timeout for select() is specified in argv[1] */

   if (!strcmp(argv[1], "-"))
      pto = NULL;                   /* Infinite timeout */
   else {
      pto = &timeout;
      timeout.tv_sec = atol(argv[1]);
      timeout.tv_usec = 0;          /* No microsecond */
   }

   /* Process remaining arguments to build file descriptor sets */

   nfds = 0;
   FD_ZERO(&readfds);
   FD_ZERO(&writefds);

   for (i=2; i<argc; i++) {
      numRead = sscanf(argv[1], "%d%2[rw]", &fd, buffer);
      if (numRead != 2)
         usageError(argv[0]);
      if (fd >= FD_SETSIZE)
         raiseError("file descriptor exceeds limit");

      if (fd >= nfds)
         nfds = fd + 1;             /* Record maximum fd + 1 */
      if (strchr(buffer, 'r') != NULL)
         FD_SET(fd, &readfds);
      if (strchr(buffer, 'w') != NULL)
         FD_SET(fd, &writefds);
   }

   /* We've built all of the arguments; now call select() */

   ready = select(nfds, &readfds, &writefds, NULL, pto); /* Ignore exceptional events */

   if (ready == -1)
      raiseError("read()");

   /* Display results of select() */

   printf("ready = %d\n", ready);

   for (fd=0; fd<nfds; fd++)
      printf("%d: %s%s\n", fd, FD_ISSET(fd, &readfds) ? "r" : "", 
             FD_ISSET(fd, &writefds) ? "w" : "");

   if (pto != NULL)
      printf("timeout after select(): %ld.%03ld\n", 
             (long) timeout.tv_sec, (long) timeout.tv_usec / 10000);

   exit(EXIT_SUCCESS);
}
