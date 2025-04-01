/* Creating a POSIX shared memory object */

#include "../linux.h"

static void usageError(const char *progName)
{
   fprintf(stderr, "Usage: %s [-cx] name size [octal-perms]\n", progName);
   fprintf(stderr, " -c    Create shared memory (O_CREAT)\n");
   fprintf(stderr, " -x    Create exclusively (O_EXCL)\n");
   exit(EXIT_FAILURE);
}

void main(int argc, char *argv[])
{
   int flags, opt, fd;
   mode_t perms;
   size_t size;
   void *addr;

   flags |= O_RDWR;
   while ((opt = getopt(argc, argv, "cx")) != -1) {
      switch (opt) {
         case 'c':   flags |= O_CREAT;    break;
         case 'x':   flags |= O_EXCL;     break;
         default:    usageError(argv[0]);
      }
   }

   if (optind + 1 >= argc)
      usageError(argv[0]);

   size = atol(argv[optind + 1]);
   perms = (argc <= optind + 2) ? (S_IRUSR | S_IWUSR) : atol(argv[optind + 2]);

   /* Create shared memory object and set its size */

   fd = open(argv[optind], flags, perms);
   if (fd == -1)
      syscall_error();

   if (ftruncate(fd, size) == -1)
      syscall_error();

   /* Map shared memory object */

   addr = mmap(NULL, size, PROT_READ | PROT_WRITE, MAP_SHARED, fd, 0);
   if (addr == MAP_FAILED)
      syscall_error();

   exit(EXIT_SUCCESS);
}
