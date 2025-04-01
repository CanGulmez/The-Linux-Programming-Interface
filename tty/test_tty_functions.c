/* Demonstrating cbreak and raw modes */

#include "../linux.h"

static struct termios userTermios;  /* Terminal settings as defined by user */

/* General handler: restore tty settings and exit */

static void handler(int sig)
{
   if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &userTermios) == -1)
      syscall_error();

   _exit(EXIT_SUCCESS);
}

/* Handler for SIGTSTP */

static void tstpHandler(int sig)
{
   struct termios ourTermios;       /* To save our tty settings */
   sigset_t tstpMask, prevMask;
   struct sigaction sa;
   int savedErrno;

   savedErrno = errno;              /* We might change 'errno' here */

   /* Save current terminal settings, restore terminal to
      state at time of program startup */

   if (tcgetattr(STDIN_FILENO, &ourTermios) == -1)
      syscall_error();
   if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &userTermios) == -1)
      syscall_error();

   /* Set the disposition of SIGTSTP to the default, raise the signal
      once more, and then unblock it so that we actually stop */

   if (signal(SIGTSTP, SIG_DFL) == SIG_ERR)
      syscall_error();
   raise(SIGTSTP);

   sigemptyset(&tstpMask);
   sigaddset(&tstpMask, SIGTSTP);
   if (sigprocmask(SIG_UNBLOCK, &tstpMask, &prevMask) == -1)
      syscall_error();

   /* Execution resumes here after SIGCONT */

   if (sigprocmask(SIG_SETMASK, &prevMask, NULL) == -1)
      syscall_error();           /* Reblock SIGTSTP */

   sigemptyset(&sa.sa_mask);     /* Reestablish handler */
   sa.sa_flags = SA_RESTART;
   sa.sa_handler = tstpHandler;
   if (sigaction(SIGTSTP, &sa, NULL) == -1)
      syscall_error();

   /* The user may have changed the terminal settings while we were
      stopped; save the settings so we can restore them later */
   
   if (tcgetattr(STDIN_FILENO, &userTermios) == -1)
      syscall_error();
   
   /* Restore our terminal settings */
   
   if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &ourTermios) == -1)
      syscall_error();

   errno = savedErrno;
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

/* Place terminal referred to by 'fd' in raw mode (noncanonical mode
   with all input and output processing disabled). Return 0 on success,
   or -1 on error. If 'prevTermios' is non-NULL, then use the buffer to
   which it points to return the previous terminal settings. */

int ttySetRaw(int fd, struct termios *prevTermios)
{
   struct termios t;

   if (tcgetattr(fd, &t) == -1)
      return -1;

   if (prevTermios != NULL)
      *prevTermios = t;

   t.c_lflag &= ~(ICANON | ISIG | IEXTEN | ECHO);
                  /* Noncanonical mode, disable signals, extended
                     input processing, and echoing */
   
   t.c_iflag &= ~(BRKINT | ICRNL | IGNBRK | IGNCR | INLCR |
                  INPCK | ISTRIP | IXON | PARMRK);
                  /* Disable special handling of CR, NL, and BREAK.
                     No 8th-bit stripping or parity error handling.
                     Disable START/STOP output flow control. */
   t.c_oflag &= ~OPOST;      /* Disable all output processing */
   t.c_cc[VMIN] = 1;
   t.c_cc[VTIME] = 0;         /* Character-at-a-time input */
                              /* with blocking */
   if (tcsetattr(fd, TCSAFLUSH, &t) == -1)
      return -1;
   
   return 0;
}
   
void main(int argc, char *argv[])
{
   char ch;
   struct sigaction sa, prev;
   ssize_t n;

   sigemptyset(&sa.sa_mask);
   sa.sa_flags = SA_RESTART;

   if (argc > 1) {            /* Use cbreak mode */
      if (ttySetCbreak(STDIN_FILENO, &userTermios) == -1)
         syscall_error();

      /* Terminal special characters can generate signals in cbreak
         mode. Catch them so that we can adjust the terminal mode.
         We establish handlers only if the signals are not being ignored. */

      sa.sa_handler = handler;

      if (sigaction(SIGQUIT, NULL, &prev) == -1)
         syscall_error();
      if (prev.sa_handler != SIG_IGN)
         if (sigaction(SIGQUIT, &sa, NULL) == -1)
            syscall_error();

      if (sigaction(SIGINT, NULL, &prev) == -1)
         syscall_error();

      if (prev.sa_handler != SIG_IGN)
         if (sigaction(SIGINT, &sa, NULL) == -1)
            syscall_error();

      sa.sa_handler = tstpHandler;

      if (sigaction(SIGTSTP, NULL, &prev) == -1)
         syscall_error();

      if (prev.sa_handler != SIG_IGN)
         if (sigaction(SIGTSTP, &sa, NULL) == -1)
            syscall_error();
   } else {                  /* Use raw mode */
      if (ttySetRaw(STDIN_FILENO, &userTermios) == -1)
         syscall_error();
   }

   sa.sa_handler = handler;
   if (sigaction(SIGTERM, &sa, NULL) == -1)
      syscall_error();

   setbuf(stdout, NULL);         /* Disable stdout buffering */

   for (;;) {                    /* Read and echo stdin */
      n = read(STDIN_FILENO, &ch, 1);
      if (n == -1) {
         syscall_error();
      }

      if (n == 0)                /* Can occur after terminal disconnect */
         break;

      if (isalpha((unsigned char) ch))    /* Letters --> lowercase */
         putchar(tolower((unsigned char) ch));
      else if (ch == '\n' || ch == '\r')
         putchar(ch);
      else if (iscntrl((unsigned char) ch))
         printf("^%c", ch ^ 64);          /* Echo Control-A as ^A, etc. */
      else
         putchar('*');                    /* All other chars as '*' */
      
      if (ch == 'q')                      /* Quit loop */
         break;
   }

   if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &userTermios) == -1)
      syscall_error();

   exit(EXIT_SUCCESS);
}
