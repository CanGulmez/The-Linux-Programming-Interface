/* Moitoring changes in the terminal window size */

#include "./utils.h"

void sigwinchHandler(int sig)
{

}

void main(int argc, char *argv[])
{
   struct winsize ws;
   struct sigaction sa;

   sigemptyset(&sa.sa_mask);
   sa.sa_flags = 0;
   sa.sa_handler = sigwinchHandler;
   if (sigaction(SIGWINCH, &sa, NULL) == -1)
      raiseError("sigaction()");

   for (;;) 
   {
      pause();                      /* Wait for SIGWINCH signal */

      if (ioctl(STDIN_FILENO, TIOCGWINSZ, &ws) == -1)
         raiseError("ioctl()");
      printf("Caught SIGWINCH, new window size: "
             "%d rows * %d columns\n", ws.ws_row, ws.ws_col);
   }
}
