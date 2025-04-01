/* Using the `dlopen` API */

#include "../linux.h"

void main(int argc, char *argv[])
{
   void *libHandle;           /* Handle for shared library */
   void (*funcp)(void);       /* POinter to function with no arguments */
   const char *err;

   if (argc != 3 || strcmp(argv[1], "--help") == 0)
      usage_error("Wrong command-line usage");

   /* Load the shared library and get a handle for later use */

   libHandle = dlopen(argv[1], RTLD_LAZY);
   if (libHandle == NULL)
      syscall_error();

   /* Search library for symbol named in argv[2] */

   (void) dlerror();          /* Clear dlerror() */
   *(void **) (&funcp) = dlsym(libHandle, argv[2]);
   err = dlerror();
   if (err != NULL)
      syscall_error();

   /* If the address returned by dlsym() is non-NULL, try calling
      it as a function that takes no arguments */

   if (funcp == NULL)
      printf("%s is NULL\n", argv[2]);
   else
      (*funcp)();

   dlclose(libHandle);       /* CLose the library */

   exit(EXIT_SUCCESS);
}
