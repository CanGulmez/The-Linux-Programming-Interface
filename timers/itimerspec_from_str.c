/* Converting time-plus-interval string to an 'itimerspec' value */

#include "../linux.h"

/* Convert a string of the following form to an itimerspec structure:
   "value.sec[/value.nanosec][:interval.sec[/interval.nanosec]]".
   Optional components that are omitted cause 0 to be assigned to the
   corresponding structure fields. */

void itimerspecFromStr(char *str, struct itimerspec *tsp)
{
   char *cptr, *sptr;

   cptr = strchr(str, ':');
   if (cptr != NULL)
      *cptr = '\0';

   sptr = strchr(str, '/');
   if (sptr != NULL)
      *sptr = '\0';
 
   tsp->it_value.tv_sec = atoi(str);
   tsp->it_value.tv_nsec = (sptr != NULL) ? atoi(sptr + 1) : 0;

   if (cptr == NULL) {
      tsp->it_interval.tv_sec = 0;
      tsp->it_interval.tv_nsec = 0;
   } else {
      sptr = strchr(cptr + 1, '/');
      if (sptr != NULL)
         *sptr = '\0';
      tsp->it_interval.tv_sec = atoi(cptr + 1);
      tsp->it_interval.tv_nsec = (sptr != NULL) ? atoi(sptr + 1) : 0;
   }
}
