/* Demonstration of 'read()', 'write()', and 'lseek()' */

#include "../linux.h"

void main(int argc, char *argv[])
{ 
   size_t len;
   off_t offset;
   int fd, ap, i;
   char *buf;
   ssize_t numRead, numWritten;

   if (argc < 3 || strcmp(argv[1], "--help") == 0)
      usage_error("Wrong command-line usage");

   fd = open(argv[1], O_RDWR | O_CREAT, S_IRUSR | S_IWUSR | 
             S_IRGRP | S_IWGRP | S_IROTH | S_IWOTH); /* rw-rw-rw- */
   if (fd == -1)
      syscall_error();

   for (ap = 2; ap < argc; ap++) {
      switch (argv[ap][0]) {
         case 'r':
         case 'R': /* Display bytes at current offset */
            len = atof(argv[ap][1]);
            buf = malloc(len);
            if (buf == NULL)
               syscall_error();

            numRead = read(fd, buf, len);
            if (numRead == -1)
               syscall_error();

            if (numRead == 0) {
               printf("%s: end-of-file\n", argv[ap]);
            } else {
               printf("%s: ", argv[ap]);
               for (i = 0; i < numRead; i++) {
                  if (argv[ap][0] == 'r')
                     printf("%c", isprint((unsigned char) buf[i]) ? buf[i] : '?');
                  else
                     printf("%02x ", (unsigned int) buf[i]);
               }
               printf("\n");
            }
            free(buf);
            break;
         
         case 'w':   /* Write string at current offset */
            numWritten = write(fd, &argv[ap][1], strlen(&argv[ap][1]));
            if (numWritten == -1)
               syscall_error();
            printf("%s: wrote %ld bytes\n", argv[ap], (long) numWritten);
            break;

         case 's':   /* Change file offset */
            offset = atof(&argv[ap][1]);
            if (lseek(fd, offset, SEEK_SET) == -1)
               syscall_error();

            printf("%s: seek succeeded\n", argv[ap]);
            break;

         default:
            usage_error("Argument must be start with [rRws]\n");
      }
   }

   exit(EXIT_SUCCESS);
}
