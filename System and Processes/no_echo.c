/* Disabling terminal echoing */

#include "./utils.h"

#define BUF_SIZE     100

void main(int argc, char *argv[])
{
   struct termios tp, save;
   char buffer[BUF_SIZE];

   /* Retrieve current terminal settings, turn echoing off */

   if (tcgetattr(STDOUT_FILENO, &tp) == -1)
      raiseError("tcgetattr()");
   save = tp;                       /* So we can restore settings later */
   tp.c_lflag &= ~ECHO;             /* ECHO off, other bits unchanged */
   if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &tp) == -1)
      raiseError("tcsetattr()");

   /* Read some input and then display it back to the user */

   printf("Enter text: ");
   fflush(stdout);
   if (fgets(buffer, BUF_SIZE, stdin) == NULL)
      printf("Got end-of-file/error on gets()\n");
   else
      printf("\nRead: %s", buffer);

   /* Restore original terminal settings */

   if (tcsetattr(STDIN_FILENO, TCSANOW, &save) == -1)
      raiseError("tcsetattr()");

   exit(EXIT_SUCCESS);
}
