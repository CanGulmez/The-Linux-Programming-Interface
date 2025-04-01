/* Using signal-driven I/O on a terminal */

#include "../linux.h"

static volatile sig_atomic_t gotSigio = 0;

static void sigioHandler(int sig)
{
   gotSigio = 1;
}

/* Place terminal referred to by 'fd' in cbreak mode (noncanonical mode
   with echoing turned off). This function assumes that the terminal is
   currently in cooked mode (i.e., we shouldn't call it if the terminal
   is currently in raw mode, since it does not undo all of the changes
   made by the ttySetRaw() function below). Return 0 on success, or -1
   on error. If 'prevTermios' is non-NULL, then use the buffer to which
   it points to return the previous terminal settings. */

int ttySetCbreak(int fd, struct termios *prevTermios)
{
   struct termios t;

   if (tcgetattr(fd, &t) == -1)
      return -1;
   
   if (prevTermios != NULL)
      *prevTermios = t;
   
   t.c_lflag &= ~(ICANON | ECHO);
   t.c_lflag |= ISIG;
   t.c_iflag &= ~ICRNL;
   t.c_cc[VMIN] = 1;
   t.c_cc[VTIME] = 0;

   if (tcsetattr(fd, TCSAFLUSH, &t) == -1)
      return -1;

   return 0;
}

void main(int argc, char *argv[])
{
   int flags, i, cnt;
   struct termios origTermios;
   char ch;
   struct sigaction sa;
   unsigned char done;

   /* Establish handler for "I/O possible" signal */

   sigemptyset(&sa.sa_mask);
   sa.sa_flags = SA_RESTART;
   sa.sa_handler = sigioHandler;
   if (sigaction(SIGIO, &sa, NULL) == -1)
      syscall_error();

   /* Set owner process that is to receive "I/O possible" signal */

   if (fcntl(STDIN_FILENO, F_SETOWN, getpid()) == -1)
      syscall_error();

   /* Enable "I/O possible" signaling and make I/O nonblocking
      for file descriptor */

   flags = fcntl(STDIN_FILENO, F_GETFL);
   if (fcntl(STDIN_FILENO, F_SETFL, flags | O_ASYNC | O_NONBLOCK) == -1)
      syscall_error();

   /* Place terminal in cbreak mode */

   if (ttySetCbreak(STDIN_FILENO, &origTermios) == -1)
      syscall_error();

   for (done = FALSE, cnt = 0; !done ; cnt++) {
      for (j = 0; j < 100000000; j++)
         continue;         /* Slow main loop down a little */
         
      if (gotSigio) {      /* Is input available? */
         /* Read all available input until error (probably EAGAIN)
            or EOF (not actually possible in cbreak mode) or a
            hash (#) character is read */

         while (read(STDIN_FILENO, &ch, 1) > 0 && !done) {
            printf("cnt=%d; read %c\n", cnt, ch);
            done = ch == '#';
         }
         gotSigio = 0;
      }
   }
   
   /* Restore original terminal settings */
            
   if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &origTermios) == -1)
      syscall_error();

   exit(EXIT_SUCCESS);
}
