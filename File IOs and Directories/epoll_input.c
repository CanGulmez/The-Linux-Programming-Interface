/* Using the 'epoll' API */

#include "./utils.h"

#define MAX_BUF      1000  /* Maximum bytes fetched by a single read() */
#define MAX_EVENTS   5     /* Maximum number of events to be returned 
                              from a single epoll_wait() call */

void main(int argc, char *argv[])
{
   int epfd, ready, fd, s, i, numOpenFds;
   struct epoll_event ev;
   struct epoll_event evlist[MAX_EVENTS];
   char buffer[MAX_BUF];

   if (argc < 2 || !strcmp(argv[1], "--help"))
      raiseError("wrong command-line arguments");

   epfd = epoll_create(argc - 1);
   if (epfd == -1)
      raiseError("epoll_create()");

   /* Open each file on command line, and add it to the "interest list" 
      for the epoll instance */

   for (i=1; i<argc; i++) {
      fd = open(argv[i], O_RDONLY);
      if (fd == -1)
         raiseError("open()");
      printf("Opened \"%s\" on fd %d\n", argv[i], fd);

      ev.events = EPOLLIN;       /* Only interest in input events */
      ev.data.fd = fd;
      if (epoll_ctl(epfd, EPOLL_CTL_ADD, fd, &ev) == -1)
         raiseError("epoll_ctl()");
   }

   numOpenFds = argc - 1;

   while (numOpenFds > 0) {

      /* Fetch up to MAX_EVENTS items from the ready list */

      printf("About to epoll_wait()\n");
      ready = epoll_wait(epfd, evlist, MAX_EVENTS, -1);
      if (ready == -1) {
         if (errno == EINTR)
            continue;         /* Restart if interrupted by signal */
         else
            raiseError("epoll_wait()");
      }

      printf("Read: %d\n", ready);

      /* Deal with returned list of events */

      for (i=0; i<ready; i++) {
         printf("   fd=%d; events: %s%s%s\n", evlist[i].data.fd, 
                (evlist[i].events & EPOLLIN) ? "EPOLLIN" : "",
                (evlist[i].events & EPOLLHUP) ? "EPOLLHUP" : "", 
                (evlist[i].events & EPOLLERR) ? "EPPOLERR" : "");

         if (evlist[i].events & EPOLLIN) {
            s = read(evlist[i].data.fd, buffer, MAX_BUF);
            if (s == -1)
               raiseError("read()");
            printf("   read %d bytes: %.*s\n", s, s, buffer);
         }
         else if (evlist[i].events & (EPOLLHUP | EPOLLERR)) {

            /* If EPOLLIN and EPOLLHUP were both set, then there might
               be more than MAX_BUF bytes to read. Therefore, we close
               the file descriptor only if EPOLLIN was not set.
               We'll read further bytes after the next epoll_wait(). */

            printf("   closing fd %d\n", evlist[i].data.fd);
            if (close(evlist[i].data.fd) == -1)
               raiseError("close()");
            
            numOpenFds --;
         }
      }
   }

   printf("All file descriptors closed; bye\n");
   exit(EXIT_SUCCESS);
}