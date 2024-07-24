/* Displaying the data from a process accounting file */

#include "./utils.h"

#define TIME_BUF_SIZE   100

/* Return name corresponding to 'uid', or NULL on error */
char *userNameFromId(uid_t uid)
{
   struct passwd *pwd;

   pwd = getpwuid(uid);
   return (pwd == NULL) ? NULL : pwd->pw_name;
}

long long comptToLL(comp_t ct)  /* Convert comp_t value into long long */
{
   const int EXP_SIZE = 3;          /* 3-bit, base-8 exponent */
   const int MANTISSA_SIZE = 13;    /* Followed by 13-bit mantissa */
   const int MANTISSA_MASK = (1 << MANTISSA_SIZE) - 1;
   long long mnatissa, exp;

   mnatissa = ct & MANTISSA_MASK;
   exp = (ct >> MANTISSA_SIZE) & ((1 << EXP_SIZE) - 1);

   return mnatissa << (exp * 3);    /* Power of 8 = left shift 3 bits */
}

void main(int argc, char *argv[])
{
   int acctFile;
   struct acct ac;
   ssize_t numRead;
   char *s;
   char timeBuffer[TIME_BUF_SIZE];
   struct tm *loc;
   time_t t;

   if (argc != 2 || !strcmp(argv[1], "--help"))
      raiseError("wrong command-line argument"); 
 
   acctFile = open(argv[1], O_RDONLY);
   if (acctFile == -1)
      raiseError("open()");

   printf("command  flags   term.  user      "
          "start time              CPU   elapsed\n");
   printf("                 status           "
          "                        time     time\n");

   while ((numRead = read(acctFile, &ac, sizeof(struct acct))) > 0)
   {
      if (numRead != sizeof(struct acct))
         raiseError("read()");

      printf("%-8.8s   ", ac.ac_comm);

      printf("%c\n", (ac.ac_flag & AFORK) ? 'F': '-');
      printf("%c\n", (ac.ac_flag & ASU) ? 'S': '-');
      printf("%c\n", (ac.ac_flag & AXSIG) ? 'X': '-');
      printf("%c\n", (ac.ac_flag & ACORE) ? 'C': '-');

#ifdef __linux__
      printf(" %#6lx   ", (unsigned long) ac.ac_exitcode);
#else
      printf(" %#6lx   ", (unsigned long) ac.ac_stat);
#endif

      s = userNameFromId(ac.ac_uid);
      printf("%-8.8s ", (s == NULL) ? "???" : s);

      t = ac.ac_btime;
      loc = localtime(&t);
      if (loc == NULL)
         printf("???Unknown time???   ");
      else
      {
         strftime(timeBuffer, TIME_BUF_SIZE, "%Y-%m-%d %T ", loc);
         printf("%s ", timeBuffer);
      }

      printf("%5.2f %7.2f ", (double) (comptToLL(ac.ac_utime) +
             comptToLL(ac.ac_stime)) / sysconf(_SC_CLK_TCK), 
             (double) comptToLL(ac.ac_etime) / sysconf(_SC_CLK_TCK));
      printf("\n"); 
   }

   if (numRead == -1)
      raiseError("read()");

   exit(EXIT_SUCCESS);
}
