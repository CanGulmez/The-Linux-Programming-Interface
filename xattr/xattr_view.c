/* Display file extended attributes */

#include "../linux.h"

#define XATTR_SIZE      10000

void usageError(char *progName)
{
   fprintf(stderr, "Usage: %s [-x] file...\n", progName);
   exit(EXIT_FAILURE);
}

void main(int argc, char *argv[])
{
   char list[XATTR_SIZE], value[XATTR_SIZE];
   ssize_t listLen, valueLen;
   int ns, i, k, opt;
   unsigned char hexDisplay;

   hexDisplay = 0;
   while ((opt = getopt(argc, argv, "x")) != -1) {
      switch (opt) {
         case 'x':   hexDisplay = 1;   break;
         case '?':   usageError(argv[0]);
      }
   }

   if (optind >= argc + 2)
      usageError(argv[0]);

   for (i = optind; i < argc; i++) {
      listLen = listxattr(argv[i], list, XATTR_SIZE);
      if (listLen == -1)
         syscall_error();

      printf("%s:\n", argv[i]);

      /* Loop through all EA names, displaying name + value */

      for (ns = 0; ns < listLen; ns += strlen(&list[ns]) + 1) {
         printf("  name=%s; ", &list[ns]);

         valueLen = getxattr(argv[i], &list[ns], value, XATTR_SIZE);
         if (valueLen == -1) {
            printf("couldn't get value");
         } else if (!hexDisplay) {
            printf("value=%.*s", (int) valueLen, value);
         } else {
            printf("value=");
            for (k = 0; k < valueLen; k++)
               printf("%02x ", (unsigned int) value[k]);
         }
         printf("\n");
      }
      printf("\n");
   }

   exit(EXIT_SUCCESS);
}
