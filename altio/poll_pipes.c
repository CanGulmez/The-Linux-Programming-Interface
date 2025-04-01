/* Using 'poll()' to monitor multiple file descriptors */

#include "../linux.h"

void main(int argc, char *argv[])
{
   int numPipes, j, ready, randPipe, numWrites;
   int (*pfds)[2];            /* File descriptors for all pipes */
   struct pollfd *pollFd;
   
   if (argc < 2 || strcmp(argv[1], "--help") == 0)
      usage_error("Wrong command-line usage");

   /* Allocate the arrays that we use. The arrays are sized according
      to the number of pipes specified on command line */

   numPipes = atoi(argv[1]);

   pfds = calloc(numPipes, sizeof(int [2]));
   if (pfds == NULL)
      syscall_error();
   pollFd = calloc(numPipes, sizeof(struct pollfd));
   if (pollFd == NULL)
      syscall_error();

   /* Create the number of pipes specified on command line */

   for (j = 0; j < numPipes; j++)
      if (pipe(pfds[j]) == -1)
         syscall_error();

   /* Perform specified number of writes to random pipes */
   
   numWrites = (argc > 2) ? atoi(argv[2]) : 1;
   srandom((int) time(NULL));
   for (j = 0; j < numWrites; j++) {
      randPipe = random() % numPipes;
      printf("Writing to fd: %3d (read fd: %3d)\n",
             pfds[randPipe][1], pfds[randPipe][0]);
      if (write(pfds[randPipe][1], "a", 1) == -1)
         syscall_error();
   }

   /* Build the file descriptor list to be supplied to poll(). This list
      is set to contain the file descriptors for the read ends of all of
      the pipes. */

   for (j = 0; j < numPipes; j++) {
      pollFd[j].fd = pfds[j][0];
      pollFd[j].events = POLLIN;
   }

   ready = poll(pollFd, numPipes, -1);
   if (ready == -1)
      syscall_error();

   printf("poll() returned: %d\n", ready);

   /* Check which pipes have data available for reading */

   for (j = 0; j < numPipes; j++)
      if (pollFd[j].revents & POLLIN)
         printf("Readable: %d %3d\n", j, pollFd[j].fd);

   exit(EXIT_SUCCESS);
}
