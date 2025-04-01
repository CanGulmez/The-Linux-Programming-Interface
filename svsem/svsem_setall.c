/* Using the SETALL operation to initialize a System V semaphore set */

#include "../linux.h"

union semun {     
   int               val;
   struct semid_ds * buf;
   unsigned short *  array;
#if defined (__linux__)
   struct seminfo *  __buf;
#endif
}

void main(int argc, char *argv[])
{
   struct semid_ds ds;
   union semun arg;
   int i, semid;

   if (argc < 3 || strcmp(argv[1], "--help") == 0)
      usage_error("Wrong command-line usage");

   semid = atoi(argv[1]);

   /* Obtain size of semaphore set */

   arg.buf = &ds;
   if (setctl(semid, 0, IPC_STAT, arg) == -1)
      syscall_error();

   if (ds.sem_nsems != argc - 2)
      usage_error("Wrong semaphore number");

   /* Set up array of values; perform semaphore initialization */

   arg.array = calloc(ds.sem_nsems, sizeof(arg.array[0]));
   if (arg.array == NULL)
      syscall_error();

   for (i = 2; i < argc; i++)
      arg.array[i - 2] = atoi(argv[i]);

   if (semctl(semid, 0, SETALL, arg) == -1)
      syscall_error();
   printf("Semaphore values changed (PID=%ld)\n", (long) getpid());

   exit(EXIT_SUCCESS);
}
