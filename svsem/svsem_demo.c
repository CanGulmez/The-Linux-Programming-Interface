/* Creating and operating on System V semaphores */

#include "../linux.h"

#define BUF_SIZE  1000

union semun {        /* Used in calls to semctl() */
   int               val;
   struct semid_ds * buf;
   unsigned short *  array;
#if defined (__linux__)
   struct seminfo *  __buf;
#endif
}

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

void main(int argc, char *argv[])
{
   int semid;

   if (argc < 2 || argc > 3 || strcmp(argv[1], "--help") == 0)
      usage_error("Wrong command-line usage");

   if (argc == 2) {          /* Create and initialize semaphore */
      union semun arg;

      semid = semget(IPC_PRIVATE, 1, S_IRUSR | S_IWUSR);
      if (semid == -1)
         syscall_error();

      arg.val = atoi(argv[1]);
      if (semctl(semid, /* semnum= */ 0, SETVAL, arg) == -1)
         syscall_error();
      
      printf("Semaphore ID = %d\n", semid);
   } else {                /* Perform an operation on first semaphore */

      struct sembuf sop;   /* Structure defining operation */

      semid = atoi(argv[1]);
      sop.sem_num = 0;     /* Specifies first semaphore in set */
      sop.sem_op = atoi(argv[2]);   /* Add, subtract, or wait for 0 */
      sop.sem_flg = 0;     /* No special options for operation */

      printf("%ld: about to semop at %s\n", (long) getpid(), currTime("%T"));
      if (semop(semid, &sop, 1) == -1)
         syscall_error();

      printf("%ld: semop completed at %s\n", (long) getpid(), currTime("%T"));
   }

   exit(EXIT_SUCCESS);
}
