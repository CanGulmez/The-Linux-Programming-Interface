/* Linux Programming Interface */

#define _GNU_SOURCE
#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>
#include <fcntl.h>
#include <sys/stat.h>
#include <string.h>
#include <errno.h>
#include <signal.h>
#include <ctype.h>
#include <sys/uio.h>
#include <pwd.h>
#include <grp.h>
#include <shadow.h>
#include <limits.h>
#include <sys/fsuid.h>
#include <time.h>
#include <sys/time.h>
#include <locale.h>
#include <sys/times.h>
#include <limits.h>
#include <sys/utsname.h>
#include <malloc.h>
#include <sys/mount.h>
#include <sys/types.h>
#include <sys/xattr.h>
#include <dirent.h>
#include <ftw.h>
#include <libgen.h>
#include <sys/inotify.h>
#include <sys/signalfd.h>
#include <pthread.h>
#include <sys/timerfd.h>
#include <sys/wait.h>
#include <sys/acct.h>
#include <sched.h>
#include <pthread.h>
#include <sys/resource.h>
#include <sched.h>
#include <sys/resource.h>
#include <sys/capability.h>
#include <utmpx.h>
#include <paths.h>
#include <lastlog.h>
#include <dlfcn.h>
#include <limits.h>
#include <ctype.h>
#include <sys/types.h>
#include <sys/ipc.h>
#include <sys/msg.h>
#include <sys/sem.h>
#include <sys/shm.h>
#include <stddef.h>
#include <sys/mman.h>
#include <mqueue.h>
#include <semaphore.h>
#include <sys/un.h>
#include <sys/socket.h>
#include <netinet/in.h>
#include <arpa/inet.h>
#include <netdb.h>
#include <termios.h>
#include <sys/ioctl.h>
#include <sys/times.h>
#include <sys/select.h>
#include <poll.h>
#include <sys/epoll.h>
#include <stdlib.h>

#define syscall_error() do {                                                     \
   fprintf(stderr, "*** %s (%s::%d) ***\n", strerror(errno), __FILE__, __LINE__);\
   exit(EXIT_FAILURE);                                                           \
} while (0)

#define usage_error(err_msg) do {                                        \
   fprintf(stderr, "*** %s (%s::%d) ***\n", err_msg, __FILE__, __LINE__);\
   exit(EXIT_FAILURE);                                                   \
} while (0)
