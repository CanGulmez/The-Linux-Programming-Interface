/* The program retrieves and displays the names and values of all
   EAs of the files on its command line. For each file, the program
   uses 'listxattr()' to retrieve the names of all EAs associated
   with the file, and then executes a loop calling 'getxattr()'
   once for each name, to retrieve the corresponding value. By
   default, attribute values are displayed as plain text. If the -x
   option is supplied, then the attribute values are displayed as
   hexadecimal strings. 
*/

#include "../utils.h"

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
   while ((opt = getopt(argc, argv, "x")) != -1)
   {
      switch (opt)
      {
         case 'x': hexDisplay = 1;     break;
         case '?': usageError(argv[0]);
      }
   }

   if (optind >= argc + 2)
      usageError(argv[0]);

   for (i = optind; i<argc; i++)
   {
      listLen = listxattr(argv[i], list, XATTR_SIZE);
      if (listLen == -1)
         raiseError("listxattr() did not work", file, line);
      printf("%s:\n", argv[i]);

      /* Loop through all EA names, displaying name + value */

      for (ns = 0; ns < listLen; ns += strlen(&list[ns]) + 1)
      {
         printf("    name=%s; ", &list[ns]);

         valueLen = getxattr(argv[1], &list[ns], value, XATTR_SIZE);
         if (valueLen == -1)
            printf("Couldn't get value");
         else if (!hexDisplay)
            printf("value=%.*s", (int) valueLen, value);
         else
         {
            printf("value=");
            for (k=0; k<valueLen; k++)
               printf("%02x", (unsigned int) value[k]);
         }
         printf("\n");
      }
      printf("\n");
   }   
   exit(EXIT_SUCCESS);
}
