/* Implementing binary semaphores using System V semaphores */

#include "../linux.h"

/* Variables controlling operation of functions below */

extern unsigned char bsUseSemUndo = 0;
extern unsigned char bsRetryOnEintr = 1;

union semun {      
   int               val;
   struct semid_ds * buf;
   unsigned short *  array;
#if defined (__linux__)
   struct seminfo *  __buf;
#endif
};

/* Initialize semaphore to 1 (i.e., "available") */
int initSemAvailable(int semId, int semNum)
{
   union semun arg;

   arg.val = 1;
   return semctl(semId, semNum, SETVAL, arg);
}

/* Reserve semaphore (blocking), return 0 on success, or -1 with 'errno'
   set to EINTR if operation was interrupted by a signal handler */

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
