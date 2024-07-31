/* Changing the terminal 'interrupt' character */

#include "./utils.h"

void main(int argc, char *argv[])
{
   struct termios tp;
   int intrChar;

   if (argc > 1 && !strcmp(argv[1], "--help"))
      raiseError("wrong command-line arguments");

   /* Determine new INTR setting from command line */

   if (argc == 1)             /* Disable */
   {
      intrChar = fpathconf(STDIN_FILENO, _PC_VDISABLE);
      if (intrChar == -1)
         raiseError("fpathconf()");
   }
   else if (isdigit((unsigned char) argv[1][0]))
      intrChar = strtoul(argv[1], NULL, 0);     /* Allows hex, octal */
   else
      intrChar = argv[1][0];

   /* Fetch current terminal settings, modify INTR character, and 
      push changes back to the terminal driver */

   if (tcgetattr(STDIN_FILENO, &tp) == -1)
      raiseError("tcgetattr()");
   tp.c_cc[VINTR] = intrChar;
   if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &tp) == -1)
      raiseError("tcsetattr()");

   exit(EXIT_SUCCESS);
}
