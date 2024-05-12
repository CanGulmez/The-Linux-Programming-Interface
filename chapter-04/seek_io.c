/* This program demonstrades the use of 'lseek()' in conjunction
   with 'read()' and 'write()'. The fist commond-line argument
   to this program is the name of a file to be opened. The remaining
   arguments specify I/O operations to be performed on the file. */

#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <assert.h>
#include <string.h>
#include <fcntl.h>
#include <ctype.h>
#include <sys/stat.h>

void main(int argc, char *argv[])
{
   off_t offset;      
   int len, fd, ap, i;
   char *buf;
   ssize_t numRead, numWritten;

   assert (argc >= 3 && strcmp(argv[1], "--help"));

   fd = open(argv[1], O_RDWR | O_CREAT, 
             S_IRUSR | S_IRUSR | S_IRGRP | S_IWGRP |
             S_IWOTH | S_IROTH);      // rw-rw-rw-
   assert (fd != -1);

   for (ap = 2; ap < argc; ap ++) {
      switch (argv[ap][0]) {
         case 'r':   // Display bytes at current offset, as text.
         case 'R':   // Display bytes at current offset, in hex.
            len = atol(&argv[ap][1]);
            buf = malloc(len);
            assert (buf != NULL);

            numRead = read(fd, buf, len);
            assert (numRead != -1); 

            if (numRead == 0) {
               printf("%s: end-of-file\n", argv[ap]);
            } else {
               printf("%s: ", argv[ap]);
               for (i=0; i<numRead; i++) {
                  if (argv[ap][0] == 'r')
                     printf("%c", isprint((unsigned char) buf[i]) ?
                                 buf[i] : '?');
                  else
                     printf("%02x ", (unsigned int) buf[i]);
               }
               printf("\n");
            }
            free(buf);
            break;
         
         case 'w':   // Write string at current offset.
            numRead = write(fd, &argv[ap][1], strlen(&argv[ap][1]));
            assert (numRead != -1);

            printf("%s: write %ld bytes\n", argv[ap], numWritten);
            break;

         case 's':   // Change file offset.
            offset = atol(&argv[ap][1]);
            assert (lseek(fd, offset, SEEK_SET) != -1);

            printf("%s: seek succeeded\n", argv[ap]);
            break;
         
         default:
            assert (0);
      }
   }
   exit(EXIT_SUCCESS);
}
