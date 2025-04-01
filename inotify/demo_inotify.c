/* Using the 'inotify()' API */

#include "../linux.h"

#define BUF_LEN (10 * (sizeof(struct inotify_event) + NAME_MAX + 1))

/* Display information from inotify_event structure */
void displayInotifyEvent(struct inotify_event *inotify)
{
      printf("    wd = %2d; ", inotify->wd);
      if (inotify->cookie > 0)
         printf("cookie = %4d; ", inotify->cookie);
   
      printf("mask = ");
      if (inotify->mask & IN_ACCESS)
         printf("IN_ACCESS ");
      if (inotify->mask & IN_ATTRIB)
         printf("IN_ATTRIB ");
      if (inotify->mask & IN_CLOSE_NOWRITE)
         printf("IN_CLOSE_NOWRITE ");
      if (inotify->mask & IN_CLOSE_WRITE)
         printf("IN_CLOSE_WRITE ");
      if (inotify->mask & IN_CREATE)
         printf("IN_CREATE ");
      if (inotify->mask & IN_DELETE)
         printf("IN_DELETE ");
      if (inotify->mask & IN_DELETE_SELF)
         printf("IN_DELETE_SELF ");
      if (inotify->mask & IN_IGNORED)
         printf("IN_IGNORED ");
      if (inotify->mask & IN_ISDIR)
         printf("IN_ISDIR ");
      if (inotify->mask & IN_MODIFY)
         printf("IN_MODIFY ");
      if (inotify->mask & IN_MOVE_SELF)
         printf("IN_MOVE_SELF ");
      if (inotify->mask & IN_MOVED_FROM)
         printf("IN_MOVED_FROM ");
      if (inotify->mask & IN_MOVED_TO)
         printf("IN_MOVED_TO ");
      if (inotify->mask & IN_OPEN)
         printf("IN_OPEN ");
      if (inotify->mask & IN_Q_OVERFLOW)
         printf("IN_Q_OVERFLOW ");
      if (inotify->mask & IN_UNMOUNT)
         printf("IN_UNMOUNT ");
      printf("\n");
   
      if (inotify->len > 0)
         printf("    name = %s\n", inotify->name);
}

void main(int argc, char *argv[])
{
   int inotifyFd, wd, i;
   char buffer[BUF_LEN];
   ssize_t numRead;
   char *p;
   struct inotify_event *event;

   if (argc < 2 || strcmp(argv[1], "--help") == 0)
      usage_error("Wrong command-line usage");

   inotifyFd = inotify_init();
   if (inotifyFd == -1)
      syscall_error();

   for (i = 1; i < argc; i++) {
      wd = inotify_add_watch(inotifyFd, argv[i], IN_ALL_EVENTS);
      if (wd == -1)
         syscall_error();

      printf("Watching %s using wd %d\n", argv[i], wd);
   }

   for (;;) {
      numRead = read(inotifyFd, buffer, BUF_LEN);  /* Read events forever */
      if (numRead == 0 | numRead == -1)
         syscall_error();

      printf("Read %ld bytes from inotify fd\n", (long) numRead);
      
      /* Process all of the events in buffer returned by read() */

      for (p = buffer; p < buffer + numRead; ) {
         event = (struct inotify_event *) p;
         displayInotifyEvent(event);

         p += sizeof(struct inotify_event) + event->len;
      }
   }

   exit(EXIT_SUCCESS);
}
