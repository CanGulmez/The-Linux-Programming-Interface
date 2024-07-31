/* Demonstrating cbreak and modes */

#include "./utils.h"

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

   t.c_cc[VMIN] = 1;                /* Character-at-a-time input */
   t.c_cc[VTIME] = 0;               /* with blocking */

   if (tcsetattr(fd, TCSAFLUSH, &t) == -1)
      return -1; 

   return 0;
}

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
   t.c_lflag &= ~(BRKINT | ICRNL | IGNBRK | IGNCR | INLCR |
                  INPCK | ISTRIP | IXON | PARMRK);
                     /* Disable special handling of CR, NL, and BREAK.
                        Disable START/STOP output flow control. */
   t.c_oflag &= ~OPOST;       /* Disable all output processing */
   t.c_cc[VMIN] = 1;          /* Character-at-a-time input */
   t.c_cc[VTIME] = 0;         /* with blocking */

   if (tcsetattr(fd, TCSAFLUSH, &t) == -1)
      return -1;

   return 0;
}

struct termios userTermios;   /* Terminal settings as defined by user */

/* General handler: restore tty settings and exit */
void handler(int sig)
{
   if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &userTermios) == -1)
      raiseError("tcsetattr()");
   _exit(EXIT_SUCCESS);
}

/* Handler for SIGTSTP */
void tstpHandler(int sig)
{
   struct termios ourTermios;       /* To save our tty settings */
   sigset_t tstpMask, prevMask;
   struct sigaction sa;
   int savedErrno;

   savedErrno = errno;              /* We might change 'errno' here */

   /* Save current terminal settings, restore terminal to
      state at time of program startup */

   if (tcgetattr(STDIN_FILENO, &ourTermios) == -1)
      raiseError("tcgetattr()");
   if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &userTermios) == -1)
      raiseError("tcsetattr()");

   /* Set the disposition of SIGTSTP to the default, raise the signal
      once more, and then unblock it so that we actually stop */

   if (signal(SIGTSTP, SIG_DFL) == SIG_ERR)
      raiseError("signal()");
   raise(SIGTSTP);

   sigemptyset(&tstpMask);
   sigaddset(&tstpMask, SIGTSTP);
   if (sigprocmask(SIG_UNBLOCK, &tstpMask, &prevMask) == -1)
      raiseError("sigprocmask()");

   /* Execution resumes here after SIGCONT */

   if (sigprocmask(SIG_SETMASK, &prevMask, NULL) == -1)
      raiseError("sigprocmask()");     /* Reblock SIGTSTP */

   sigemptyset(&sa.sa_mask);           /* Reestablish handler */
   sa.sa_flags = SA_RESTART;
   sa.sa_handler = tstpHandler;
   if (sigaction(SIGTSTP, &sa, NULL) == -1)
      raiseError("sigaction()");

   /* The user may have changed the terminal settings while we were
      stopped; save the settings so we can restore them later */

   if (tcgetattr(STDIN_FILENO, &userTermios) == -1)
      raiseError("tcgetattr()");

   /* Restore our terminal settings */

   if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &ourTermios) == -1)
      raiseError("tcsetattr()");

   errno = savedErrno;
}

void main(int argc, char *argv[])
{
   char ch;
   struct sigaction sa, prev;
   ssize_t n;

   sigemptyset(&sa.sa_mask);
   sa.sa_flags = SA_RESTART;

   if (argc > 1)              /* Use cbreak mode */
   {
      if (ttySetCbreak(STDIN_FILENO, &userTermios) == -1)
         raiseError("ttySetCbreak()");

      /* Terminal special characters can generate signals in cbreak
         mode. Catch them so that we can adjust the terminal mode.
         We establish handlers only if the signals are not being ignored. */

      sa.sa_handler = handler;

      if (sigaction(SIGQUIT, NULL, &prev) == -1)
         raiseError("sigaction()");
      if (prev.sa_handler != SIG_IGN)
         if (sigaction(SIGQUIT, &sa, NULL) == -1)
            raiseError("sigaction()");

      if (sigaction(SIGINT, NULL, &prev) == -1)
         raiseError("sigaction()");
      if (prev.sa_handler != SIG_IGN)
         if (sigaction(SIGINT, &sa, NULL) == -1)
            raiseError("sigaction()");

      sa.sa_handler = tstpHandler;

      if (sigaction(SIGTSTP, NULL, &prev) == -1)
         raiseError("sigaction()");
      if (prev.sa_handler != SIG_IGN)
         if (sigaction(SIGTSTP, &sa, NULL) == -1)
            raiseError("sigaction()");
   }
   else                             /* Use raw mode */
      if (ttySetRaw(STDIN_FILENO, &userTermios) == -1)
         raiseError("ttySetRaw()");

   sa.sa_handler = handler;
   if (sigaction(SIGTERM, &sa, NULL) == -1)
      raiseError("sigaction()");

   setbuf(stdout, NULL);      /* Disable stdout buffering */

   for (;;)                   /* Read and echo stdin */
   {
      n = read(STDIN_FILENO, &ch, 1);
      if (n == -1) 
      {
         raiseError("read()");
         break;
      }
      if (n == 0)             /* Can occur after terminal disconnect */
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
      raiseError("tcsetattr()");

   exit(EXIT_SUCCESS);
}
