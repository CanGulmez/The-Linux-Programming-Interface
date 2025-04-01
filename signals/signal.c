/* An implementation of 'signal()' */

#include "../linux.h"

typedef void (*handler_t)(int);

handler_t custom_signal(int sig, handler_t handler)
{
   struct sigaction newDisp, prevDisp;

   newDisp.sa_handler = handler;
   sigemptyset(&newDisp.sa_mask);
#ifdef OLD_SIGNAL
   newDisp.sa_flags = SA_RESETHAND | SA_NODEFER;
#else
   newDisp.sa_flags = SA_RESTART;
#endif

   if (sigaction(sig, &newDisp, &prevDisp) == -1)
      return SIG_ERR;
   else
      return prevDisp.sa_handler;
}
