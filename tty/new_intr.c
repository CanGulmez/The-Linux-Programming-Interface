/* Changing the terminal 'interrupt' character */

#include "../linux.h"

void main(int argc, char *argv[])
{
   struct termios tp;
   int intrChar;

   if (argc > 1 && strcmp(argv[1], "--help") == 0)
      usage_error("Wrong command-line usage");

   /* Determine new INTR setting from command line */

   if (argc == 1) {
      intrChar = fpathconf(STDIN_FILENO, _PC_VDISABLE);
      if (intrChar == -1)
         syscall_error();
   } else if (isdigit((unsigned char) argv[1][0])) {
      intrChar = strtoul(argv[1], NULL, 0);  /* Allow hex, octal */  
   } else {                                  /* Literal character */
      intrChar = argv[1][0];
   }

   /* Fetch current terminal settings, modify INTR character, and
      push changes back to the terminal driver */

   if (tcgetattr(STDIN_FILENO, &tp) == -1)
      syscall_error();
   tp.c_cc[VINTR] = intrChar;
   if (tcsetattr(STDIN_FILENO, TCSAFLUSH, &tp) == -1)
      syscall_error();

   exit(EXIT_SUCCESS);
}
