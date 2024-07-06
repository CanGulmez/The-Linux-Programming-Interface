/* The program demonstrates the use of 'lseek()' in conjunction with 
   'read()' and 'write()'. The first command-line argument to this 
   program is the name of a file to be opened. The remaining arguments
   specify I/O operations to be performed on the file. Each of these
   operations consists of a letter followed by an associated value (
   no separating space):

   + s-offset-    : seek to byte offset from the start of the file
   + r-lenght-    : read lenght bytes from the file, starting at the 
                    current file offset, and display them in text.
   + R-lenght-    : read lenght bytes from the file, starting at the 
                    current file offset, and display them in hex.
   + w-str-       : write the string of characters specified in str 
                    at current file offset.
*/

#include "../utils.h"
 
void main(int argc, char *argv[])
{
   size_t len;
   off_t offset;
   int fd, ap, i;
   char *buf;
   ssize_t numRead, numWritten;

   if (argc < 3 || !strcmp(argv[1], "--help"))
      raiseError("wrong command-line arguments", file, line);

   fd = open(argv[1], O_RDWR | O_CREAT, 
             S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP |
             S_IROTH | S_IWOTH);    /* rw-rw-rw- */
   if (fd == -1)
      raiseError("open() did not work", file, line);
 
   for (ap = 2; ap < argc; ap++)
   {
      switch (argv[ap][0])
      {
         case 'r':      /* Display bytes at current offset, as text */
         case 'R':      /* Display bytes at current offset, in hex */
            len = atol(&argv[ap][1]);
 
            buf = malloc(len);
            if (buf == NULL)
               raiseError("malloc() did not work", file, line);
 
            numRead = read(fd, buf, len);
            if (numRead == -1)
               raiseError("read() did not work", file, line);

            if (numRead == 0)
               printf("%s: end-of-file\n", argv[ap]);
            else
            {
               printf("%s: ", argv[ap]);
               for (i=0; i<numRead; i++)
               {
                  if (argv[ap][0] == 'r')
                     printf("%c", isprint((unsigned char) buf[i]) ?
                            buf[i] : '?');
                  else
                     printf("%02x", (unsigned int) buf[i]);
               }
               printf("\n");
            }
            free(buf);
            break;

         case 'w':         /* Write string at current offset */
            numWritten = write(fd, &argv[ap][1], strlen(&argv[ap][1]));
            if (numWritten == -1)
               raiseError("write() did not work", file, line);

            printf("%s: write %ld bytes\n", argv[ap], (long) numWritten);
            break;

         case 's':         /* Change file offset */
            offset = atol(&argv[ap][1]);
            if (lseek(fd, offset, SEEK_SET) == -1)
               raiseError("lseek() did not work", file, line);

            printf("%s: seek succeeded\n", argv[ap]);
            break;

         default:
            raiseError("wrong command-line arguments", file, line);
      }
   }

   exit(EXIT_SUCCESS);
}
