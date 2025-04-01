/* Transfer blocks of data from 'stdin' to a System V shared memory segment */

#include "../linux.h"

#define SHM_KEY   0x1234      /* Key for shared memory segment */
#define SEM_KEY   0x5678      /* Key for semaphore set */

#define OBJ_PERMS (S_IRUSR | S_IWUSR | S_IRGRP | S_IWGRP)
                              /* Permissions for our IPC objects */

#define WRITE_SEM 0           /* Writer has access to shared memory */
#define READ_SEM  1           /* Reader has access to shared memory */

#ifndef BUF_SIZE              /* Allow "cc -D" to override defination */
#define BUF_SIZE  1024        /* Size of transfer buffer */
#endif

union semun {      
   int               val;
   struct semid_ds * buf;
   unsigned short *  array;
#if defined (__linux__)
   struct seminfo *  __buf;
#endif
};

struct shmseg {               /* Defines structure of shared memory segment */
   int ctn;                   /* Number of bytes used in 'buf' */
   char buf[BUF_SIZE];        /* Data being transferred */
};

/* Initialize semaphore to 1 (i.e., "available") */
int initSemAvailable(int semId, int semNum)
{
   union semun arg;

   arg.val = 1;
   return semctl(semId, semNum, SETVAL, arg);
}

/* Reserve semaphore - decrement it by 1 */
int reserveSem(int semId, int semNum)
{
   struct sembuf sops;

   sops.sem_num = semNum;
   sops.sem_op = -1;
   sops.sem_flg = bsUseSemUndo ? SEM_UNDO : 0;

   while (semop(semId, &sops, 1) == -1)
      if (errno != EINTR || !bsRetryOnEintr)
         return -1;

   return 0;
}

/* Release semaphore - increment it by 1 */
int releaseSem(int semId, int semNum)
{
   struct sembuf sops;

   sops.sem_num = semNum;
   sops.sem_op = 1;
   sops.sem_flg = bsUseSemUndo ? SEM_UNDO : 0;

   return semop(semId, &sops, 1);
}

void main(int argc, char *argv[])
{
   int semid, shmid, bytes, xfrs;
   struct shmseg *shmp;
   union semun dummy;

   semid = semget(SEM_KEY, 2, IPC_CREAT | OBJ_PERMS);
   if (semid == -1)
      syscall_error();

   if (initSemAvailable(semid, WRITE_SEM) == -1)
      syscall_error();
   if (initSemInUse(semid, READ_SEM) == -1)
      syscall_error();
      
   shmid = shmget(SHM_KEY, sizeof(struct shmseg), IPC_CREAT | OBJ_PERMS);
   if (shmid == -1)
      syscall_error();  

   shmp = shmat(shmid, NULL, 0);
   if (shmp == (void *) -1)
      syscall_error();

   /* Transfer blocks of data from stdin to shared memory */

   for (xfrs = 0, bytes = 0; ; xfrs++, bytes += shmp->ctn) {
      if (reserveSem(semid, WRITE_SEM) == -1)
         syscall_error();

      shmp->ctn = read(STDIN_FILENO, shmp->buf, BUF_SIZE);
      if (shmp->ctn == -1)
         syscall_error();

      if (releaseSem(semid, READ_SEM) == -1)
         syscall_error();

      /* Have we reached EOF? We test this after giving the reader
         a turn so that it can see the 0 value in shmp->cnt. */

      if (shmp->ctn == 0)
         break;
   }

   /* Wait until reader has let us have one more turn. We then know 
      reader has finished, and so we can delete the IPC objects. */

   if (reserveSem(semid, WRITE_SEM) == -1)
      syscall_error();

   if (shmctl(semid, 0, IPC_RMID, dummy) == -1)
      syscall_error();
   if (shmdt(shmp) == -1)
      syscall_error();
   if (shmctl(shmid, IPC_RMID, 0) == -1)
      syscall_error();

   fprintf(stderr, "Sent %d bytes (%d xfrs)\n", bytes, xfrs);
   exit(EXIT_SUCCESS);
}
