/* Displaying process resource limits */

#include "./utils.h"

/* Print 'msg' followed by limits for 'resource' */
int printRlimit(const char *msg, int resource)
{
   struct rlimit rlim;

   if (getrlimit(resource, &rlim) == -1)
      return -1;

   printf("%s soft=", msg);
   if (rlim.rlim_cur == RLIM_INFINITY)
      printf("infinite");
   else if (rlim.rlim_cur == RLIM_SAVED_CUR)
      printf("unrepresentable");
   else
      printf("%lld", (long long) rlim.rlim_cur);

   printf("; hard=");
   if (rlim.rlim_max == RLIM_INFINITY) 
      printf("infinite\n");
   else if (rlim.rlim_max == RLIM_SAVED_MAX)
      printf("unrrepresentable");
   else
      printf("%lld\n", (long long) rlim.rlim_max);

   return 0;
}
