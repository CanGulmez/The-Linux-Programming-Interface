/* A semaphore monitoring program */

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
   union semun arg, dummy;
   int semid, i;

   if (argc != 2 || strcmp(argv[1], "--help") == 0)
      usage_error("Wrong command-line usage");

   semid = atoi(argv[1]);

   arg.buf = &ds;
   if (semctl(semid, 0, IPC_STAT, arg) == -1)
      syscall_error();

   printf("Semaphore changed: %s", ctime(&ds.sem_ctime));
   printf("Last semop():      %s", ctime(&ds.sem_otime));

   /* Display per-semaphore information */

   arg.array = calloc(ds.sem_nsems, sizeof(arg.array[0]));
   if (arg.array == NULL)
      syscall_error();
   if (semctl(semid, 0, GETALL, arg) == -1)
      syscall_error();

   printf("Sem #  Value  SEMPID   SEMCNT  SEMZCNT\n");

   for (i = 0; i < ds.sem_nsems; i++)
      printf("%3d  %5d  %5d  %5d   %5d\n", i, arg.array[i],
         semctl(semid, i, GETPID, dummy), 
         semctl(semid, i, GETNCNT, dummy),
         semctl(semid, i, GETZCNT, dummy));

   exit(EXIT_SUCCESS);
}
