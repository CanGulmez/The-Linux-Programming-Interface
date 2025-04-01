/* Transfer blocks of data from a System V shared memory segment to 'stdout' */

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
   int semid, shmid, xfrs, bytes;
   struct shmseg *shmp;

   /* Get IDs for semaphore set and shared memory created by writer */

   semid = semget(SEM_KEY, 0, 0);
   if (semid == -1)
      syscall_error();

   shmid = shmget(SHM_KEY, 0, 0);
   if (shmid == -1)
      syscall_error();

   shmp = shmat(shmid, NULL, SHM_RDONLY);
   if (shmp == (void *) -1)
      syscall_error();

   /* Transfer blocks of data from shared memory to stdout */

   for (xfrs = 0, bytes = 0; ; xfrs++) {
      if (reserveSem(semid, READ_SEM) == -1)
         syscall_error();

      if (shmp->ctn == 0)
         break;
      bytes += shmp->ctn;

      if (write(STDOUT_FILENO, shmp->buf, shmp->ctn) != shmp->ctn)
         syscall_error();

      if (releaseSem(semid, WRITE_SEM) == -1)
         syscall_error();      
   }

   if (shmdt(shmp) == -1)
      syscall_error();

   /* Give writer one more turn, so it can clean up */

   if (releaseSem(semid, WRITE_SEM) == -1)
      syscall_error();

   fprintf(stderr, "Received %d bytes (%d xfrs)\n", bytes, xfrs);
   exit(EXIT_SUCCESS);
}
