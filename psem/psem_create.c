/* Using 'sem_open()' to open or create a POSIX named semaphore */

#include "../linux.h"

static void usageError(const char *progName)
{
   fprintf(stderr, "Usage: %s [-cx] name [octal-perms [value]]\n", progName);
   fprintf(stderr, "    -c       Create semaphore (O_CREAT)\n");
   fprintf(stderr, "    -x       Create exclusively (O_EXCL)\n");
   exit(EXIT_FAILURE);
}

void main(int argc, char *argv[])
{
   int flags, opt;
   mode_t perms;
   unsigned int value;
   sem_t *sem; 

   flags = 0;
   while ((opt = getopt(argc, argv, "cx")) != -1) {
      switch (opt) {
         case 'c':
            flags |= O_CREAT;
            break;
         case 'x':
            flags |= O_EXCL;
            break;
         default:
            usageError(argv[0]);
      }
   }

   if (optind >= argc)
      usageError(argv[0]);

   /* Default permissions are rw-------; default semaphore initialization
      value is 0 */

   perms = (argc <= optind + 2) ? (S_IRUSR | S_IWUSR) : 
      atoi(argv[optind + 1]);
   value = (argc <= optind + 2) ? 0 : atoi(argv[optind + 2]);

   sem = sem_open(argv[optind], flags, perms, value);
   if (sem == SEM_FAILED)
      syscall_error();

   exit(EXIT_SUCCESS);
}
