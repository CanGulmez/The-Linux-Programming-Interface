/* Creating and operating on System V semaphores */

#include "./utils.h"

union semun {           /* Used in calls to semctl() */
   int                  val;
   struct semid_ds *    buf;
   unsigned short *     array;
#if defined(__linux__)
   struct seminfo *     __buf;
#endif
};

char *currTime(const char *format)
{
   static char buffer[BUFSIZ];      /* Noreentrant */
   time_t t;
   size_t s;
   struct tm *tm;

   t = time(NULL);
   tm = localtime(&t);
   if (tm == NULL)   
      return NULL;
 
   s = strftime(buffer, BUFSIZ, (format != NULL) ? format : "%c", tm);

   return (s == 0) ? NULL : buffer;
}

void main(int argc, char *argv[])
{
   int semid;

   if (argc < 2 || argc > 3 || !strcmp(argv[1], "--help"))
      raiseError("wrong command-line argument");

   if (argc == 2) {             /* Create and intialize semaphore */
      union semun arg;

      semid = semget(IPC_PRIVATE, 1, S_IRUSR | S_IWUSR);
      if (semid == -1)
         raiseError("semget()");

      arg.val = atoi(argv[1]);
      if (semctl(semid, /* semun= */ 0, SETVAL, arg) == -1)
         raiseError("semctl()");

      printf("Semaphore ID = %d\n", semid);
   } 
   else {                /* Perform an operation on first semaphore */
      struct sembuf sop;            /* Structure defining operation */

      sop.sem_num = 0;              /* Specifies fisrt semaphore in set */
      sop.sem_op = atoi(argv[2]);   /* Add, subtract, or wait for 0 */
      sop.sem_flg = 0;              /* No special options for operation */

      printf("%ld: about to semop at %s\n", (long) getpid(), currTime("%T"));
      if (semop(semid, &sop, 1) == -1)
         raiseError("semop()");

      printf("%ld: semop completed at %s\n", (long) getpid(), currTime("%T"));
   }

   exit(EXIT_SUCCESS);
}
