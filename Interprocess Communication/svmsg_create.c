/* Using 'msgget()' */

#include "./utils.h"

/* Print usage info, then exit */ 
void usageError(char *progName, char *msg)
{
   if (msg != NULL)
      fprintf(stderr, "%s", msg);
   fprintf(stderr, "Usage: %s [-cx] {-f pathname | -k key | -p} "
           "[octal-perms]\n", progName);
   fprintf(stderr, "    -c          Use IPC_CREAT flag\n");
   fprintf(stderr, "    -x          Use IPC_EXCL flag\n");
   fprintf(stderr, "    -f pathname Generate key using ftok()\n");
   fprintf(stderr, "    -k key      Use 'key' as key\n");
   fprintf(stderr, "    -p          Use IPC_PRIVATE key\n");
   exit(EXIT_FAILURE);
}

void main(int argc, char *argv[])
{
   int numKeyFlags;        /* Counts -f, -k, and -p options */
   int flags, msgid, opt;
   unsigned int perms;
   long lkey;
   key_t key;

   /* Parse command-line options and arguments */

   numKeyFlags = 0;
   flags = 0;

   while ((opt = getopt(argc, argv, "cf:k:px")) != -1) {
      switch (opt) {

         case 'c':
            flags |= IPC_CREAT;
            break;

         case 'f':         /* -f pathname */
            key = ftok(optarg, 1); 
            if (key == -1)
               raiseError("ftok()");
            numKeyFlags ++;
            break;
         
         case 'k':         /* -k key (octal, decimal or hexadecimal) */
            if (sscanf(optarg, "%li", &key) != 1)
               raiseError("sscanf()");
            key = lkey;
            numKeyFlags ++;
            break;

         case 'p':
            key = IPC_PRIVATE;
            numKeyFlags ++;
            break;

         case 'x':
            flags |= IPC_EXCL;
            break;

         default:
         usageError(argv[0], "Bad option\n");
      }
   }

   if (numKeyFlags != 1)
      usageError(argv[0], "Exactly one of the options -f, -k, "
                 "or -p must be supplied\n");

   perms = (optind == argc) ? (S_IRUSR | S_IWUSR) :
           atoi(argv[optind]); 

   msgid = msgget(key, flags | perms);
   if (msgid == -1)
      raiseError("msgget()");

   printf("%d\n", msgid);

   exit(EXIT_SUCCESS);
}
