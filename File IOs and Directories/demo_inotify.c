/* The program demonstrates the use of 'inotify'. */

#include "../utils.h"

#define BUF_LEN   (10 * (sizeof(struct inotify_event) + NAME_MAX + 1))

/* Display information from inotify_event structure */
void displayInotifyEvent(struct inotify_event *i)
{
   printf("    wd =%2d; ", i->wd);
   if (i->cookie > 0)   
      printf("cookie =%4d; ", i->cookie);

   printf("mask = ");
   if (i->mask & IN_ACCESS)         printf("IN_ACCESS ");
   if (i->mask & IN_ATTRIB)         printf("IN_ATTRIB ");
   if (i->mask & IN_CLOSE_NOWRITE)  printf("IN_CLOSE_NOWRITE ");
   if (i->mask & IN_CLOSE_WRITE)    printf("IN_CLOSE_WRITE ");
   if (i->mask & IN_CREATE)         printf("IN_CREATE ");
   if (i->mask & IN_DELETE)         printf("IN_DELETE ");
   if (i->mask & IN_DELETE_SELF)    printf("IN_DELETE_SELF ");
   if (i->mask & IN_IGNORED)        printf("IN_IGNORED ");
   if (i->mask & IN_ISDIR)          printf("IN_ISDIR ");
   if (i->mask & IN_MODIFY)         printf("IN_MODIFY ");
   if (i->mask & IN_MOVE_SELF)      printf("IN_MOVE_SELF ");
   if (i->mask & IN_MOVED_FROM)     printf("IN_MOVED_FROM ");
   if (i->mask & IN_MOVED_TO)       printf("IN_MOVED_TO ");
   if (i->mask & IN_OPEN)           printf("IN_OPEN ");
   if (i->mask & IN_Q_OVERFLOW)     printf("IN_Q_OVERFLOW ");
   if (i->mask & IN_UNMOUNT)        printf("IN_UNMOUNT ");
   printf("\n");

   if (i->len > 0)
      printf("       name = %s\n", i->name);
}

void main(int argc, char *argv[])
{
   int inotifyFd, wd, i;
   char buf[BUF_LEN];
   ssize_t numRead;
   char *p;
   struct inotify_event *event;

   if (argc < 2 || !strcmp(argv[1], "--help"))
      raiseError("wrong command-line arguments", file, line);

   inotifyFd = inotify_init();      /* Create inotify instance */
   if (inotifyFd == -1)
      raiseError("inotify_init() did not work", file, line);

   for (i=1; i<argc; i++)
   {
      wd = inotify_add_watch(inotifyFd, argv[i], IN_ALL_EVENTS);
      if (wd == -1)
         raiseError("inotify_add_watch() did not work", file, line);

      printf("Watching %s using wd %d\n", argv[i], wd);
   }

   for (;;)                         /* Read events forever */
   {
      numRead = read(inotifyFd, buf, BUF_LEN);
      if (numRead == 0)
         raiseError("read() did not work", file, line);
      
      if (numRead == -1)
         raiseError("read() did not work", file, line);

      /* Process all of the events in buffer returned by read() */

      for (p=buf; p<buf + numRead; )
      {
         event = (struct inotify_event *) p;
         displayInotifyEvent(event);

         p += sizeof(struct inotify_event) + event->len;
      }
   }

   exit(EXIT_SUCCESS);
}
