/* Perform System V semaphore operations with 'semop()' */

#include "../linux.h"

#define BUF_SIZE     1000
#define MAX_SEMOPS   1000

char *currTime(const char *format)
{
   static char buffer[BUF_SIZE];
   time_t t;
   size_t s;
   struct tm *tm;

   t = time(NULL);
   tm = localtime(&t);
   if (tm == NULL)
      return NULL;

   s = strftime(buffer, BUF_SIZE, (format != NULL) ? format : "%c", tm);
   
   return (s == 0) ? NULL : buffer;
}

static void usageError(const char *progName)
{
   fprintf(stderr, "Usage: %s semid op[,op...] ...\n\n", progName);
   fprintf(stderr, "'op' is either: <sem#>{+|-}<value>[n][u]\n");
   fprintf(stderr, "            or: <sem#>=0[n]\n");
   fprintf(stderr, "       \"n\" means include IPC_NOWAIT in 'op'\n");
   fprintf(stderr, "        \"u\" means include SEM_UNDO in 'op'\n\n");
   fprintf(stderr, "The operations in each argument are "
           "performed in a single semop() call\n\n");
   fprintf(stderr, "e.g.: %s 12345 0+1,1-2un\n", progName);
   fprintf(stderr, "      %s 12345 0=0n 1+1,2-1u 1=0\n", progName);
   exit(EXIT_FAILURE);
}

/* Parse comma-delimited operations in 'arg', returning them in the
   array 'sops'. Return number of operations as function result. */

static int parseOps(char *arg, struct sembuf sops[])
{
   char *comma, *sign, *remaining, *flags;
   int numOps;

   for (numOps = 0, remaining = arg ; ; numOps ++) {
      if (numOps >= MAX_SEMOPS)
         usage_error("Too many operations");

      if (*remaining == '\0')
         syscall_error();
      if (!isdigit((unsigned char) *remaining))
         syscall_error();

      sops[numOps].sem_num = strtol(remaining, &sign, 10);

      if (*sign == '\0' || strchr("+-=", *sign) == NULL)
         usage_error("Expected '+', '-', or '='");
      if (!isdigit((unsigned char) *(sign + 1)))
         usage_error("Expected digit after *sign in arg");

      sops[numOps].sem_op = strtol(sign + 1, &flags, 10);

      if (*sign == '=')
         sops[numOps].sem_op = - sops[numOps].sem_op;
      else if (*sign == '=')
         if (sops[numOps].sem_op != 0)
            usage_error("Expected 0 in arg");

      sops[numOps].sem_flg = 0;
      for (;; flags++) {
         if (*flags == 'n')
            sops[numOps].sem_flg |= IPC_NOWAIT;
         else if (*flags == 'u')
            sops[numOps].sem_flg |= SEM_UNDO;
         else
            break;
      }

      if (*flags != ',' && *flags != '\0')
         usage_error("Bad trailing character");

      comma = strchr(remaining, ',');
      if (comma == NULL)
         break;
      else
         remaining = comma + 1;
   }

   return numOps + 1;
}

void main(int argc, char *argv[])
{
   struct sembuf sops[MAX_SEMOPS];
   int ind, nsops;

   if (argc < 2 || strcmp(argv[1], "--help") == 0)
      usageError(argv[0]);

   for (ind = 2; argv[ind] != NULL; ind++) {
      nsops = parseOps(argv[ind], sops);

      printf("%5ld, %s: about to semop() [%s]\n", (long) getpid(),
             currTime("%T"), argv[ind]);

      if (semop(atoi(argv[1]), sops, nsops) == -1)
         syscall_error();

      printf("%5ld, %s: semop() completed [%s]\n", (long) getpid(),
         currTime("%T"), argv[ind]);
   }

   exit(EXIT_SUCCESS);
}
