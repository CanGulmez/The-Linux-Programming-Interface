/* Receiving message notification via a thread */

#include "../linux.h"

static void notifySetup(mqd_t *mqdp);

/* Thread notification function */
static void threadFunc(union sigval sv)
{
   ssize_t numRead;
   mqd_t *mqdp;
   void *buffer;
   struct mq_attr attr;

   mqdp = sv.sival_ptr;

   if (mq_getattr(*mqdp, &attr) == -1)
      syscall_error();

   buffer = malloc(attr.mq_msgsize);
   if (buffer == NULL)
      syscall_error();

   notifySetup(mqdp);

   while ((numRead = mq_receive(*mqdp, buffer, attr.mq_msgsize, NULL)) >= 0)
      printf("Read %ld bytes\n", (long) numRead);
   
   if (errno != EAGAIN)
      syscall_error();

   free(buffer);
   pthread_exit(NULL);
}

static void notifySetup(mqd_t *mqdp)
{
   struct sigevent sev;

   sev.sigev_notify = SIGEV_THREAD;    /* Notify via thread */
   sev.sigev_notify_function = threadFunc;
   sev.sigev_notify_attributes = NULL;
            /* Could be pointer to pthread_attr_t structure */
   sev.sigev_value.sival_ptr = mqdp;   /* Argument to threadFunc() */

   if (mq_notify(*mqdp, &sev) == -1)
      syscall_error();
}

void main(int argc, char *argv[])
{
   mqd_t mqd;
   
   if (argc != 2 || strcmp(argv[1], "--help") == 0)
      usage_error("Wrong command-line usage");

   mqd = mq_open(argv[1], O_RDONLY | O_NONBLOCK);
   if (mqd == (mqd_t) -1)
      syscall_error();

   notifySetup(&mqd);
   pause();
}
