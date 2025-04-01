# File I/O: THE UNIVERSAL I/O MODEL

## Overview

All system calls for performing I/O refer to open files using a
`file descriptor`. By convention, most programs expects to be able
to use the three standard file descriptors, `stdin`, `stdout` and
`stderr`.

The following are the four key system calls for performing file 
I/O:

+ `fd = open(pathname, flags, mode)`
+ `numread = read(fd, buffer, count)`
+ `numwritten = write(fd, buffer, count)`
+ `status = close(fd)`

## Universality of I/O

One of the distinguishing features of the UNIX I/O model is the 
concept of `universality of I/O`. This means that the same four
system calls are used to perform I/O on all type of files, 
including devices such as terminals.

## Changing the File Offset: `lseek()`

For each open file, the kernel records a `file offset`, sometimes
also called the `read write offset` or `pointer`. This is the 
location in the file at which the next `read()` or `write()` will
commence. 

# ----------------------------------------------------------------


# FILE I/O: FURTHER DETAILS

## Atomicity and Race Conditions

All system calls are executed atomically. By this, we mean that 
the kernel guarantees that all of the steps in a system call
completed as a single operation, without being interrupted by 
another process or thread.

A race condition is a situation where the result produced by two
processes (or threads) operating on shared resources depends in 
an excepted way on the relative order in which the processes gain
access to the CPU(s).

## File Control Operations: `fcntl()`

One use of `fcntl()` is to retrieve or modify the access mode and 
open file status flags of an open file.

## Relationship Between File Descriptors and Open Files

To understand what is going on, we need to examine three data 
structures maintained by the kernel:

+ the per-process file descriptor table;
+ the system-wide table of open file descriptors; and
+ the file system i-node table.

For each process, the kernel maintains a table of `open file`
`descriptors`. Each entry in this table records information about
a single file descriptor, including:

+ a set of flags controlling the operation of the file descriptor;
+ and a reference to the open file description.

The kernel maintains a system-wide table of all `open file`
`descriptions`. An open file description stores all information
relating to an open file, including:

+ the current file offset;
+ status flags specified when opening the file ;
+ the file access mode;
+ settings relating to signal-driven I/O; and
+ a reference to the `i-node` object for this file.

Each file system has a table of `i-nodes` for all files residing
in the file system. `i-node` for each file includes:

+ file type and permissions;
+ a pointer to a list of locks held on this file; and
+ various properties of the file, including its size and 
  timestamps relating to different types of file operations.

## Duplicating File Descriptors

The `dup()` call takes `oldfd`, an open file descriptor, and 
returns a new descriptor that refers to the same open file 
description. The new descriptor is guaranteed to be lowest unused
file description.

The `dup2()` system call makes a duplicate of the file descriptor
given in `oldfd` using the descriptor number in `newfd`. If the
file descriptor specified in `newfd` is already open, `dup2()`
closes it first.

The `dup3()` syscam call performs the same task as `dup2()`, but
adds an additional argument, `flags`, that is a bit mask that
modifies the behavior of the system call.

## Truncating a File: `truncate()` and `ftruncate()`

The `truncate()` and `ftruncate()` system calls set the size of a
file to the value specified by `length`.

If the file is longer that `length`, the excess data is lost. If
the file is currently shorter than `length`, it is extended by 
padding with a sequence of null bytes or a hole.

## Nonblocking I/O

Specifying the O_NONBLOCK flag when opening a file serves two 
putposes:

+ If the file cannot be opened immediately, then `open()` returns
  an error instead of blocking.
+ After successful `open()`, subsequent I/O operations are also
  nonblocking.

Nonblocking mode can be used with devices. O_NONBLOCK is generally
ignored for regular files.

# ----------------------------------------------------------------


# PROCESSES

## Processes and Programs

A `process` is an instance of an exectuing program. A `program`
is a file containing a range of information that describes how
to construct a process at run time. This information includes the
following:

+ `Binary format identification`
+ `Machine-language instructions`
+ `Program entry-point address`
+ `Symbol and relocation tables`
+ `Shared-library and dynamic-linking information`
+ `Other information`

A process in an abstract entity, defined by the kernel, to which
system resources are allocated in order to execute a program.

## Memory Layout of a Process

The memory allocated to each process is composed of a number of 
parts, usually referred to as `segments`. These segments are as 
follows:

+ `text segment` (machine-language instructions)
+ `initialized data segment`
+ `uninitialized data segment`
+ `stack`
+ `heap`

## Virtual Memory Management

The aim of this is to make efficient use of both the CPU and RAM
(physical memory) by exploiting a property that is typical of 
most programs: `locality of reference`. Most program demonstrate
two kinds of locality:

+ `Spatial locality`
+ `Temporal locality`

A virtual memory scheme splits the memory used by each program 
into small, fixed-size units called `pages`.

In order to support this organization, the kernel maintains a 
`page table` for each process. The page table describes the 
location of each page in the process's `virtual address space`
(the set of all virtual memory pages avaliable to the process).

If a process tries to access an address for which there is no
corresponding page-table entry, it receives a SIGSEGV signal.

## The Stack and Stack Frames

The stack grows and shrinks linearly as functions are called and
returned. The kernel stack is a per-process memory region 
maintained in kernel memory that is used as the stack for 
execution of the functions called internally during the execution
of a system call. Each stack frame contains the following 
information:

+ Function arguments and local variables,
+ Call linkage information.

## Environment List

Each process has an associated array of strings called the 
`environment list`, or simly the `environment`. A common use of
environment variables is in shell.

# ----------------------------------------------------------------


# Memory Allocation

A process can allocate memory by increasing the size of the heap,
a variable-size segment of contiguous virtual memory that begins
just after the uninitialized data segment of a process and grows
and shrinks as memory is allocated and freed. The current limit
of the heap is reffered to as  the `program break`.

## Implementation of `malloc()` and `free()`

To avoid these types of errors, we should observe the following
rules:

+ After we allocate a block of memory, we should be careful not
  to touch any bytes outside the range of that block. 
+ It is an error to free the same piece of allocated memory
  more than once.
+ We should never call `free()` with a pointer value that wasn't
  obtained by a call to one of the functions in the `malloc`
  package.
+ If we are writing a long-running program that repeatedly 
  allocates memory for various purposes, then we should ensure
  that we deallocate any memory after we have finished using it.
  Failure to do so means that the heap will steadily grow until
  we reach the limits of vavaliable virtual memory.

## Allocating Memory on the Stack: `alloca()`

Like the functions in the `malloc` package, `alloca()` allocates
memory dynamically. Howover, instead of obtaining memory from the
heap, `allocate` obtains memory from the stack by increasing the
size of the stack frame.

# ----------------------------------------------------------------


# Users and Groups

Every user has a unique login name an associated numeric user 
identifier (UID). Users can belong to one ot more groups. Each
group laso has a unique name and a group identifier.

The primary purpose of user and group IDs is to determine 
ownership of various system resources and to control permissions
granted to processes accessing those resources.

In `/etc/passwd`, each line is composed of seven fields separated
by colon. In order, these fields are as follows:

+ `Login name`,
+ `Encrypted password`,
+ `User ID (UID)`,
+ `Group ID (GID)`,
+ `Comment`,
+ `Home directory`,
+ `Login shell`.

For various administrative purposes, in particular, controlling
access to files and other system resources, it is useful to 
organize users into `groups`. The `group file`, /etc/group, 
contains one line for each group in the system. Each line consists
of four colon-separated fields. In order, these fields are as
follows:

+ `Group name`,
+ `Encrypted password`,
+ `Group ID (GID)`,
+ `User list`

Some applications require that users authenticate themselves.
Authentication typically takes the form of a username (login name)
and password. For security reasons, UNIX systems encrypt passwords
using a `one-way encryption` algorithm, which means that there is
no method of re-creating the original password from its encrypted
form.

# ----------------------------------------------------------------


# Process Credentials

Every process has a set of associated numeric user identifiers
(UIDs) and group identifiers (GIDs). These identifiers are as 
follows:

+ real user ID and group ID;
+ effective user ID and group ID;
+ saved set-user-ID and saved set-group-ID;
+ file-system user ID and group ID; and
+ supplementary group IDs.

The real user ID and group ID identify the user and group to 
which the process belongs. The effective user ID and group ID, 
in conjunction with the supplementary groups IDs, are used to 
determine the permissions granted to a process when it tries
to perform various operations. Normally, the effective user and
group IDs have the same values as the corresponding real IDs.

A set-user-ID program allows a process to gain privileges it 
would not normally have, by setting the process's effective user
ID to the same value as the user ID of the executable file.

The saved set-user-ID and saved set-group-ID are designed for 
use with set-user-ID and set-group-ID programs.

On Linux, it is the file-system user and groups IDs, rather than
the effective user and group IDs, that are used to determine
permissions when performing file-system operations. Normally,
the file-system user and group IDs have the same values as the
corresponding effective IDs.

The supplementary group IDs are a set of additional groups to
which a process belongs.

# ----------------------------------------------------------------


# Time

Within a program, we may be interested in two kinds of time:

+ `Real time`
+ `Process time`

Regardless of geographic location, UNIX systems represent time
internally as a measure of seconds since the Epoch; that is, since
midnight on the morning of 1 January 1970, Universal Coordinated 
Time (UTC, previously known as Greenwich Mean Time, or GMT).

Functions for retrieving and working with calendar time:

+ `time()`: kernel --> time_t
+ `stime()`: time_t --> kernel
+ `gettimeofday()`: kernel --> struct timeval
+ `settimeofday()`: struct timeval --> kernel
+ `ctime()`: time_t --> fixed-format string
+ `gmtime()`: time_t --> struct tm
+ `localtime()`: time_t --> struct tm
+ `mktime()`: struct tm --> time_t
+ `asctime()`: struct tm --> fized-format string
+ `strftime()`: struct tm --> user-formatted, localized string
+ `strptime()`: user-formatted, localized string --> struct tm

Selected conversion specifiers for `strftime()`:

+ %a     --> Abbreviateed weekday name --> Tue
+ %A     --> Full weekday name         --> Tuesday
+ %b, %h --> Abbreviated month name    --> Feb
+ %B     --> Full month name           --> February
+ %c     --> Date and time             --> Tue Feb 1 21:39:46 2011
+ %d     --> Day of month              --> 01
+ %D     --> American date (%m/%d/%y)  --> 02/01/11
+ %e     --> Day of month              --> 1
+ %F     --> ISO date (%Y-%m-%d)       --> 2011-02-01
+ %H     --> Hour (24-hour clock)      --> 21
+ %I     --> Hour (12-hour clock)      --> 09
+ %j     --> Day of year               --> 032
+ %m     --> Decimal month             --> 02
+ %M     --> Minute                    --> 39
+ %p     --> AM/PM                     --> PM
+ %P     --> am/pm                     --> pm
+ %R     --> 24-hour time              --> 21:36
+ %S     --> Second                    --> 46
+ %u     --> Weekday number            --> 2
+ %U     --> Sunday week number        --> 05
+ %w     --> Weekday number            --> 2
+ %W     --> Monday week number        --> 05
+ %x     --> Date (localized)          --> 02/01/11
+ %X     --> Time (localized)          --> 21:39:46
+ %y     --> 2-digit year              --> 11
+ %Y     --> 4-digit year              --> 2011
+ %Z     --> Timezone name             --> CET

Programs that input and output times must take into account the
timezones and DST regime of the system on which they are run.

SUSv3 defines a `locale` as the "subset of a user's environment
that depends on language and cultural conventions." Ideally, all
programs designed to run in more than one location shoudl deal
with locales in order to display and input information in the
user's preferred language and format. This constitutes the 
complex subject of `internationalization (I18N)`.

Process time is the amount of CPU time used by a process since 
it was created. For recording purposes, the kernel separates CPU 
time into the following two components:

+ `User CPU time` (in user mode)
+ `System CPU time` (in kernel mode)

Sometimes, we refer to process time as the `total CPU time`
consumed by the process.

The `clock_t` data types used to type the four fields of the `tms`
structure is an integer type that measures time in units called
`clock ticks`.

On success, `times()` returns the elapsed (real) time in clock
ticks since some arbitrary points in the past.

The `clock()` function provides a simpler interface for retrieving
the process time. It returns a single value that measures the 
total (user plus system) CPU time used by the calling process The 
value returned by `clock()` is measured in units CLOCKS_PER_SEC.

# ----------------------------------------------------------------


# System Limits and Options

The C programming language standards and SUSv3 provide two 
principle avenues for an application to obtain such information:

+ Some limits and options can be determined at compile time.
+ Other limits and options may vary at run time. SUSv3 defines
  three functions - `sysconf()`, `pathconf()`, `fpathconf()`.

SUSv3 requires that the value returned by `sysconf()` for a 
particular limit be constant for the lifetime of the calling 
process.

# ----------------------------------------------------------------


# System and Process Information

For each process on the system, the kernel provides a 
corresponding directory named `/proc/PID` is the ID of process.

Selected files in each `/proc/PID` directory:

+ `cmdline`   --> Command-line arguments delimited by \0
+ `cwd`       --> Symbolic link to current working directory
+ `environ`   --> Environment list NAME=value pairs
+ `exe`       --> Symbolic link to file being executed
+ `fd`        --> Directory containing symbplic links to files
                  opened by this process
+ `maps`      --> Memory mappings
+ `mem`       --> Process virtual memory
+ `mounts`    --> Mount points for this process
+ `root`      --> Symbolic link to root directory
+ `status`    --> Various information
+ `task`      --> COntains one subdirectory for each thread

Purpose of selected `/proc` subdirectories:

+ `/proc`            --> Various system information
+ `/proc/net`        --> Status information about networking
+ `/proc/sys/fs`     --> Settings related to file systems
+ `/proc/sys/kernel` --> Various general kernel settings
+ `/proc/sys/net`    --> Networking and sockets settings
+ `/proc/sys/vm`     --> Memory-management settings
+ `/proc/sysvipc`    --> Information about System V IPC object

# ---------------------------------------------------------------


# File I/O Buffering

When working with disk files, the system calls don't directly 
initiate disk access. Instead, they simply copy data between a 
user-space buffer and a buffer in the kernel `buffer cache`.

The aim of this design is to allow system calls to be fast, since
they don't need to wait on a slow disk operation.

In summary, if we are transferring a large amount of data to or 
from a file, then by buffering data in large blocks, and thus 
performing fewer system calls, we can greatly improve I/O  
performance.

Buffering of data into large blocks to reduce system calls is
exactly what is done by the C library I/O functions when operating
on disk files.

Regardless of the current buffering mode, at any time, we can
force the data in a `stdio` output stream to be written using 
`fflush()` library function.

It is possible to force flushing of kernel buffers for output 
files. Sometimes, this is necassary if an application must ensure
that output really has been written to the disk.

In summary, if need to force flushing of kernek buffers, we should
consider whether we can design our application to use large
`write()` buffer sizes or make judicious use of occasional calls
to `fsync()` or `fdatasync()`, instead of using the O_SYNC flag
when opening the file.

Linux allows an application to bypass the buffer cache when 
performing disk I/O, thus transferring data directly from user
space to a file or disk device. This is sometimes termed 
`direct I/O` or `Raw I/O`.

# ----------------------------------------------------------------


# File Systems

A device special file corresponds to a device on the system.
Within the kernel, each device type has a corresponding device
driver, which handles all I/O requests for the device. A `device`
`driver` is a unit of kernel code that implements a set of 
operations that corresponds to input and output actions on an
associated piece of hardware.

Devices can be divided into two types:

+ `Character devices` handle data on a character-by-character
  basis.
+ `Block devices` handle data a block at a time.

Each device file has a `major ID number` and a `minor ID number`,
The major ID identifies the general class of device, and is used
by the kernel to look up the appropriate dirver for this type of 
devices. The minor ID uniquely identifies a particular device 
within a general class. The major and minor IDs of a device file
are displayed by the `ls -l` command.

Each disk is divided into one or more (nonoverlapping) 
`partitions`. Each partition is treated by the kernel as a 
separate device residing under the /dev directory.

A disk partition may hold any type of information, but usually
contains one of the following:

+ a `file system` holding regular files and directories;
+ a `data area` accessed as a raw-mode device;
+ a `swap area` used by the kernel for memory management.

A file system is an organized collection of regular files and
directories. A file system is created using the `mkfs` command.

A file system contains the following parts:

+ `Boot block`: It contains information used to boot the 
                operating system.
+ `Superblock`: It contains the size of the i-node table, the
                size of locical blocks in this file system; and
                the size of the file system in logical blocks.
+ `I-node table`: Each file and directory in the file system has
                  a unique entry in the i-node table. This entry
                  records various information about the file.
+ `Data blocks`: The great majority of space in a file system is
                 used for the blocks of data that form the files
                 and directories residing in the file system.
          
The information maintained in an i-node includes the following:

+ File type;
+ Owner;
+ Group;
+ Access permissions for three categories of user,
+ Three timestamps: time of last access to the file, time of
  last modification of the file, and time of last status change;
+ Number of hard links to the file;
+ Size of the file in bytes;
+ Number of blocks actually allocated to the file, measured in
  units of 512-byte blocks;
+ Pointers to the data blocks of the file.

The `virtual file system` (VFS) is a kernel feature that create
an abstraction layer for file-system operations:

+ The VFS defines a generic interface for file system operations.
+ Each file system provides an implementation for the VFS.

Journaling file systems eliminate the need for lengthy file-system
consistency checks after a system crash. A journaling file system
logs all metadata updates to a special on-disk journal file 
before they are actually carried out.

On Linux, all files from all file systems reside under a single
directory tree. At the base of this tree is the root directory, /
(slash). Other file systems are `mounted` under the root directory
and appear as subtrees within the overall hierarchy.

The `statvfs()` and `fstatvfs()` library functions obtain 
information about a mounted file system.

# ----------------------------------------------------------------


# File Attributes

The `stat(), lstat(), fstat()` system calls retrieve information
about a file, mostly drawn from the file i-node.

Each file has an associated user ID (UID) and group ID (GID). 
These IDs determine which user and group the file belongs to. When
a new file is created, its user ID is taken from the effective 
user ID of the process.

The `chown()`, `lchown()`, `fchown()` system calls change the 
owner (user ID) and group (group ID) of a file.

The bottom 12 bits of the `st_mode` field of the `stat` structure
define the permissions for a file. The first 3 of these bits are
special bits known as the set-user-ID, set-group-ID, and sticky
bits. The remaining 9 bits from the mask defining the permissions
that are granted to various categories of users accessing the 
file. The file permissions mask divides the world into three
categories:

+ `Owner`,
+ `Group`,
+ `Other`.

Three permissions may be granted to each user category:

+ `Read`,
+ `Write`,
+ `Execute`.

Directories have the same permission scheme as files. The kernel
checks file permissions whenever we specify a pathname in a 
system call includes aa directory prefix, then in addition to 
checking for the required permissions on the file itself, the 
kernel also checks for execute permission on each of the 
directories in this prefix.

The `access()` system call checks the accessibility of the file
specified in `pathname` based ob process's real user and group
IDs (and supplementary group IDs).

For new files, the kernel uses the permissions specified in the 
`mode` argument to `open()`. For new directories, permissions are
set according to the `mode` argument to `mkdir()`.

The `umask` is a process attribute that specifies which permission
bits should always be turned `off` when new files or directories
are created by the process.

# ----------------------------------------------------------------


# Extended Attributes

EAs are used to implement access control lists and file 
capabilities. EAs have names of the form `namespace.name`. The 
`namespace` component serves to separate. EAs into functionally
distinct classes. The `name` component uniquely identifies an 
EA within the given `namespace`.

Four values are supported for `namespace`: `user`, `trusted`,
`system`, `security`. These four types of EAs are used as follows:

+ `User` EAs may be manipulated by unprivileged processes, subject
  to file permission checks.
+ `Trusted` EAs are like `user` EAs in that they can be 
  manipulated by user processes.
+ `System` EAs are used by the kernel to associate system objects
  with a file.
+ `Security` EAs are used to store file security labels for 
  operating system security modules.

# ----------------------------------------------------------------


# Directories and Links

A `directory` is stored in the file system in a similar way to a
regular file. Two things distinguish a directory from a regular 
file:

+ A directory is marked with a different file type in its i-node.
+ A directory is a file with a special organization.

All of the names (links) for a file are equivalent. None of the 
names has priority over any of the others.

A `symbolic link`, also sometimes called a `soft link`, is a 
special file type whose data is the name of another file.

Symbolic links don't have the same status as hard links. In  
particular, a symbolic link in not included in the link count of 
the file to which it refers.

Therefore, if the filename to which the symbolic link refers is
removed, the symbolic link itself continues to exist, even 
though it can no longer be deferenced. We say that it has become
a `dangling link`.

Since a symbolic link refers to a filename, rather than an i-node
number, it can be used to link a file in a different file system.

It is possible to chain symbolic links.

Every process has a `root directory`, which is the point from
which absolute pathnames are interpreted. By default, this is the
real root directory of the filesystem. On occasion, it is useful
for a process to change its root directory.

# ----------------------------------------------------------------


# Monitoring File Events

Some applications need to be able to monitor files or directories
in order to determine whether events have occured for the 
monitored objects. The key steps in the use of the `inotify` API
are as follows:

+ The application uses `inotify_init()` to create an `inotify`
  `instance`.
+ The application informs the kernel about which files are of
  interest by using `inotify_add_watch()` to add items to the
  watch list of the `inotify` instance created in the previous
  step.
+ In order to obtain event notifications, the application 
  performs `read()` operations on the `inotify` file descriptor.
+ When the application has finished monitoring, it closes the 
  `inotify` file descriptor.

# ----------------------------------------------------------------


# Signals: Fundamental Concepts

A `signal` is a notification to a process that an event has
occured. Signals are sometimes described as `software interrupts`.
In most cases, it is not possible to predict exactly when a signal
will arrive.

The usual source of many signals sent to a process is the kernel.
Amoung the types of events that cause the kernel to generate a 
signal for a process are the following:

+ A hardware exception occured.
+ The user typed one of the terminal special characters that 
  generate signals.
+ A software event occured.

Signals fall into two broad categories. The first set constitutes
the `traditional` or `standard` signals, which are used by the 
kernel to notify processes of events. On Linux, the standard 
signals are numered from 1 to 31. The other set of signals 
consists of the `realtime` signals.

A signal is said to be `generated` by some event. Once generated,
a signal is later `delivered` to a process, which then takes some 
action in response to signal. Between the time it is generated and
the the time it is delivered, a signl is said to be `pending`.

Sometimes, howover, we need to ensure that a segment of code is
not interrupted by the delivery of a signal. To do thi, we can
add a signal to the process's `signal mask`.

Upon delivery of a signal, a process carries out one of the 
following default actions, depending on the signal:

+ The signal is `ignored`.
+ The process is `terminated`.
+ A `core dump file` is generated.
+ The process is `stopped`.
+ Execution of the process is `resumed` after previously being
  stopped.

Instead of accepting the default for a particular signal, a 
program can change the action that occurs when a signal is 
delivered:

+ The `default action` should occur.
+ The signal is `ignored`.
+ A `signal handler` is executed.

In general, it is preferable to write simple signal handlers. Two
common designs for signal handlers are the following:

+ The signal handler sets a global flag and exits. The main 
  program periodically checks this flag and, if it is set, takes
  appropriate action.
+ The signal handlers performs some type of cleanup and then 
  either terminates the process or uses a nonlocal goto to unwind
  the stack and return control to a predefined location in the 
  main program.

Not all system calls and library functions can be safely called
from a signal handler. 

To explain what a reentrant function is, we need to first 
distinguish between single-threaded and multi-threaded programs.

A function is said to be `reentrant` if it can safely be 
simultaneously executed by multiple threads of execution in the
same process. In this context, "safe" means that the function 
achieves its expected result, regardless of the state of execution
of any other thread of execution.

A function may be `nonentrant` if it updates global or static data
structures. (A function that employs only local variables is 
guaranted to be reentrant.)

`malloc()` and `free()` maintain a linked list of freed memory
blocks available for reallocation from the heap. For this reason,
the `malloc()` family of functions, and other library functions
that use them, are nonreentrant.

Other library functions are nonreentrant because they return 
information using statically allocated memory.

Functions can also be nonreentrant if they use static data
structures for their internal bookkeeping. The most obvious 
examples of such functions are the members of the `stdio`.

Even if we are not using nonreentrant library functions, 
reentrancy issues can still be relevant. If a signal handler 
updates programmer-defined global data structures that are also
updated within the main program, then we can say tha the signal
handler is nonreentrant with respect to the main program.

An `async-signal-safe` function is one that the implementation
guarantees to be safe when called from a signal handler. A 
function is async-signal-safe either because it is reentrant or
because it is not interruptible by a signal handler.

In other words, when writing signal handlers, we have two choices:

+ Ensure that the code of the signal handler itself is reentrant
  and that it calls only async-signal-safe functions.
+ Block delivery of signals while executing code in the main
  program that calls unsafe functions or works with global data
  structures also updated by the signal handler.

The above rules are also simplified to the statement that we must
not call unsafe functions from within a signal handler.

The workaround is to save the value of `errno` on entry to a 
signal handler that uses any of the functions and restore the 
`errno` value on exit from the handler.

When global variables are accessed in this way from a signal 
handler, we should always declare them using the `volatile`
attribute in order to prevent the compiler from performing
optimizations that result in the variable being stored in a 
register.

The C language standards and SUSv3 specify an integer data type,
`sig_atomic_t`, for which reads and writes are guaranteed to be
atomic. Thus, a global flag variable that is shared between main
program and a signal handler should be declared as follow:
`volatile sig_atomic_t flag;`

Consider the following scenario:

+ We establish a handler for some signal.
+ We make a blocking system call.
+ While the system call is blocked, the signal for which we
  establish a handler is delivered, and its signal is invoked.

By default, the system call fails with the erro EINTR.

We can specify the SA_RESTART flag when establishing the signal
handler with `sigaction()`, so that system calls are automatically
restarted by the kernel on the process's behalf.

A core dump is a file containing a memory image of the process at
the time it terminated.

It is not possible to change the default action for SIGKILL, 
which always terminates a process, and, SIGSTOP, which always 
stops a process.

The SIGCONT signal is used to continue a process previously 
stopped by one of the stop signals.

If a process is currently stopped, the arrival of  a SIGCONT 
signal always causes the process to resume, even if the process 
is currently blocking or ignoring SIGCONT.

SIGBUS, SIGFPE, SIGILL, and SIGSEV can be generated as a 
consequence of a hardware exception.

The correct way to deal with hardware-generated signals is 
either to accept their default action or to write handlers that
don't perform a normal return.

Synchronously generated signals are delivered immediately. For
example, a hardware exception triggers an immediate signal, and
when process sends itself a signal using `raise()`, the signal
is delivered before the `raise()` call returns.

When a signal is generated asynchronously, there may be a (small)
delay while the signal is pending between the time when it was 
generated and the time it is actually delivered, even if we have
not blocked the signal. The reason for this is that the kernel 
delivers a pending signal to a process only at the next switch
from kernel mode to user mode while executing that process.

Realtime signals have the following advantages over standard 
signals:

+ Realtime signals provide an increased range of signals that 
  can be used for application-defined purposes.
+ Realtime signals are queued. If multiple instances of a 
  realtime signal are sent to a process, then the signal is 
  delivered multiple times.
+ When sending a realtime signal, it is possible to specify data.
+ The order of delivery of different realtime signals is 
  guaranteed. Signals are prioritized, with lowest-numbered 
  signals having higher priority.

The Linux kernel defines 32 different realtime signals, numbered
from 32 to 63.

The `sigsuspend()` system call replaces the process signal mask
by the signal set pointed to by `mask`, and then suspends 
execution of the process until a signal is caught and its handler
returns. Once the handler returns, `sissuspend()` restores the 
process signal mask to the value it had prior to the call.

The `sigwaitinfo()` system call suspends execution of the 
process until one of the signals in the signal set pointed by 
`set` is delivered. If one of the signals in `set` is already
pending at the time of the call, `sigwaitinfo()` returns 
immediately.

The `signalfd()` system call creates a special file descriptor
from which signals directed to the caller can be read.

# ----------------------------------------------------------------


# Timers and Sleeping

A timer allows a process to schedule a notification for itself
to occur at some time in the feature. Sleeping allows a process
to suspend execution for a peroid time.

On modern Linux kernels, timer resolution is limited by the 
frequency of the software clock no longer holds true. Linux 
optionally supports high-resolution timers. If this support is 
enabled (CONFIG_HIGH_RES_TIMERS kernel configuration option),
then the accuracy of the various timer and sleep interfaces is
no longer constrained by the size of the kernel jiffy. Instead,
these calls can be as accurate as the underlying hardware allows.
On modern hardware, accuracy down to a microsecond is typical.

One use of real-time timers is to place  an upper limit on the 
time for which a blocking system call can remainblocked. 

Sometimes, we want to suspend execution of a process for a fixed
amount of time. The `sleep()` function suspends execution of the
calling process for number of seconds.

POSIX clocks provide an API for accessing clocks that measure 
time with nanosecond precision. Nanosecond time values are 
represented using the same `timespec` structure.

The classical UNIX interval timers set by `setitimer()` suffer a
number of limitations:

+ We can set only one timer of each of the three types.
+ The only way of neing notified of timer expiration is via 
  delivery of a signal.
+ We have no way of knowing whether there was a timer overrun.
+ Timers are limited to microsecond resolution.

The POSIX timer API divides the life of a timer into the 
following steps:

+ The `timer_create()` system call creates a new timer and defines
  the method by which it will notify the process when it expires.
+ The `timer_settime()` system call starts or stops a timer.
+ The `timer_delete()` system call deletes a timer that is no
  longer required.

# ----------------------------------------------------------------


# Process Creation

+ The `fork()` system call allows one process, the parent, to 
  create a new process, the child. This is done by making the 
  new child process an (almost) exact duplicate of the parent:
  the child obtains copies of the parent't stack, data, heap and
  text segments.
+ The `exit(status)` library function terminates a process, making
  all resources used by the process available for subsequent 
  reallocation by the kernel.
+ The `wait(status)` system call two purposes. First, if a child 
  of this process has not yet terminated by calling `exit()`, then
  `wait()` suspends execution of the process until one of its
  children has terminated. Second, the termination status of the
  child is returned in the status argument of `wait()`.
+ The `execve(pathname, argv, argvp)` system call loads a new 
  program into a process's memory. The existing text is discarded
  and the stack, data, and heap segments are freshly created for
  the new program. This operation is often referred to as 
  `execing` a new program.

The key point to understand `fork()` is to realize that after it
has completed its work, two processes exist, and, in each process,
execution continues from the point where `fork()` returns.

The two processes are executing the same process text, but they
have separate copies of the stack, data, and heap segments.

It is important to realize that after a `fork()`, it is 
indeterminate which of the two processes is next scheduled to 
use the CPU.

When a `fork()` is perfromed, the child receives duplicates of
all the parent's file descritors. Corresponding descriptors in 
the parent and the child refer to the same open file descritor.
File attributes of an open file are shared between the parent 
and child.

Linux uses two techniques to avoid such wasteful copying:

+ The kernel marks the text segment of each process as read-only
  so that a process cannot copy its own code. This means that
  the parent and child can share the same text segment.
+ For pages in the data, heap and stack segments og the parent
  process, the kernel employs a techniques known as 
  `copy-on-write`.

After a `fork()`, if either process needs to wait fo the other
to complete an action, then the active process can send a signal
after completing the action; the other process waits for the 
signal.

A process may terminate in two general ways. One of these is
`abnormal` termination, caused by the delivery of a signal whose
default action is to terminate the process. Alternatively, a
process can terminate `normally`, using the `_exit()` system 
call.

The following actions are performed by `exit()`:

+ Exit handlers are called, in reverse order of their 
  registration.
+ The `stdio` stream buffers are flushed.
+ The `_exit()` system call is invoked, using the value supplied
  in `status`.

In many application designs, a parent process needs to know when
one of its child processes changes state - when the child 
terminates or is stopped by a signal. Two techniques used to
monitor child processes: the `wait()` system call and the use 
of the SIGCHLD signal.

The `wait()` system call does the following:

+ If no child of the calling process has yet terminated, the call
  blocks until one of the children terminates.
+ As its function result, `wait()` returns the process ID of the
  child that has terminated.

The `wait()` system call has a number of limitations:

+ If a parent process has created multiple children, it is not
  possible to `wait()` for completion of a specific child: we 
  can only wait for the next child that terminates.
+ If no child has yet terminated, `wait()` always blocks. 
  Sometimes, it would be preferable to perform a nonblocking 
  wait so that if no child has yet terminated, we obtain an
  immediate indication of this fact.
+ Using `wait()`, we can find out only about children that have
  terminated. It is not possible to be notified when a child is
  stopped by a signal or when a stopped child is resumed by 
  delivery of a SIGCONT signal.

The lifetimes of parent and child processes are usually not the
same. This raises two questions:

+ Who becomes the parent of an `orphaned` child. The orphaned 
  child is adopted by `init`, the ancestor of all processes.
+ What happens to a child that terminates before its parent has
  had a change to perform a `wait()`? The point here is that, 
  although the child has finished its work, the parent should
  be permitted to perform a `wait()` at some later time to 
  determinate how the child terminated.

The parent can always eventually perform a `wait()`. When the
parent does perform a `wait()`, the kernel removes the zombie,
since the last remaining information about the child is no
longer required.

The SIGCHLD signal is sent to a parent process whenever one of
its children terminates. By default, this signal is ignored, but
we can catch it by installing a signal handler.

The `execve()` system call loads a new program into a process's
memory. During this opeation, the old program is discarded, and
the process's stack, data, and heap are replaced by those of
the new program.

An interpreter is a program that reads commands in text form and
executes them. UNIX kernels allow interpreter scripts to be run
in the same way as a binary program file. The `#!` characters
must be placed at the start of the line.

By default, all file descriptors opened by a program that calls
`exec()` remain open across the `exec()` and are avaliable for 
use by the new program.

Sometimes, it may be desirable to ensure that certain file
descriptors are closed before an `exec()`. In particular, we 
`exec()` an unknown program from a prviliged process, or a 
program that doesn't need descriptors for files we have already
opened, then it is secure programming practice to ensure that
all unnecessary file descriptors are closed before the new
program is loaded.

For these reasons, the kernel provides a close-on-exec for each
file descriptor. If this flag in set, the file descriptor is
automatically closed during a successful `exec()`, but left 
open if the `exec()` flags.

During an `exec()`, the text of the existing process is 
discarded. This text may include signal handlers established by
the calling program. Because the handlers disappear, the kernel
resets the dispositions of all handled signals to SIG_DFL.

During an `exec()`, the process signal mask and set of pending
signals are both preserved. This feature allows us to block
and queue signals for the newly execed program.

The `system()` function allows the calling program to execute
an arbitrary shell command.

When process accounting is enabled, the kernel writes an 
accounting record to the system-wide process accounting file as
each process terminates. This accounting record contains various
information manitained by the kernel about the process, including
its termination status and how much CPU time is comsumed.

The Linux-specific `clone()` system call creates a new process. 
It differes from the other two calls in allowing finer control
over the steps that occur during process creation.

# ---------------------------------------------------------------


# THREADS

Like processes, threads are mechanism that permits an application
to perform multiple tasks concurrently. A signle process can 
contain nultiple threads. All of these threads are independently
executing the same program, and they all share the same global
memory, including the initialized data, uninitialized data, and
heap segments.

The threads in a process can execute concurrently. On a 
multiprocessor system, multiple threads can execute parallel.

Therefore, in threaded programs, each thread has its own `errno`
value. All Pthreads functions return 0 on success or a positive
value on failure.

The executing of a thread terminates in one of the following:

+ The thread's start function performs a return specifying a
  return value for the thread.
+ The thread calls `pthread_exit()`.
+ The thread is canceled using `pthread_cancel()`.
+ Any of the threads calls `exit()`, or the main thread 
  performs a return, which causes all threads in the process 
  to terminate immediately.

The `pthread_join()` function waits for the thread identified
by `thread` to terminate.

By default, a thread is `joinable`, meaning that when it 
terminates, another thread can obtain its return status using
`pthread_join()`. Sometimes, we don't care about the thread's
return status; we can mark the thread as `detached`, by making
a call to `pthread_detach()`.

We begin by considering the advantages of a multithreaded 
approach:

+ Sharing data between threads is easy. By contrast, sharing 
  data between processes requires more work.
+ Thread creating is faster than process creating; context-
  switch time may be lower for threads than for processes.

Using threads can have some disadvantages compared to using
processes:

+ When programming with threads, we need to ensure that the 
  functions we call are thread-safe.
+ A bug in one thread can damage all of the threads in the 
  process.
+ Each thread is competing for use of the finite virtual address
  space space of the host process.

One of the principal advantages of threads is that they can
share information via global variables.

The term `critical section` is used to refer to a section of 
code that accesses a shared resource and whose execution should
be `atomic`; that it, its execution should not be interrupted
by another thread that simultaneously accesses the same shared
resource.

Mutexes can be used to ensure atomic access to any shared 
resource, but protecting shared variables is the most common use.

A mutex has two states: `locked` and `unlocked`. At any moment,
at most one thread may hold the lock on a mutex. 

The static initializer value PTHREAD_MUTEX_INITIALIZER can be
used only for initializing a statically allocated mutex with
default attributes. In all other cases, we dynamically initialize
the mutex using `pthread_mutex_init()/destroy()`.

A mutex prevents multiple threads from accessing a shared 
variable at the same time. A condition variable allows one 
thread to inform other threads about changes in the state of 
shared variable and allows the other threads to wait for such 
information.

A condition variable is always used in conjunction with a mutex.
As with mutexes, condition variables can be allocated statically
or dynamically.

A function is said to be `thread-safe` if it can safely be 
invoked by multiple threads at the same time.

Typically, multiple threads execute in parallel, with each 
thread performing its task until it decides to terminate by 
calling `pthread_exit()` or returning from the thread's start
function.

Sometimes, it can be useful to `cancel` a thread; that is, to
send it a request asking it to terminate now.

If a thread with a pending cancellation were simply terminated
when it reached a cancellation point, then shared variables and
Pthreads objects might be left in an inconsistent state, 
perhaps causing the remaining threads in the process to produce
incorrent results, deadlock, or crash. To get around this
problem, a thread can establish one or more `cleanup handlers`-
functions that are automatically executed if the thread is
cancelled.

When any thread calls one of the `exec()` functions, the calling
program is completely replaced. All threads, except the one that
called `exec()`, vanish immediately.

When a multithreaded process calls `fork()`, only the calling
thread is replicated in the child process.

# ---------------------------------------------------------------


# Process Groups, Sessions, and Job Control

Process groups and sessions form a two-level hierarchical 
relationship between processes: a process group is a collection
of related processes, and a session is a collection of related
process groups. 

A `process group` is a set of one or more processes sharing the
same `process group identifier` (PGID). A `session` is a 
collection of process groups. A process's session membership
is determined by its `session identifier` (SID).

All of the processes in a session share a signle `controlling`
`terminal`. The controlling terminal is established when the
session leader first opens a terminal device. A terminal may be
controlling terminal of at most one session.

All of the processes in a session may have a (single) controlling
terminal. The controlling terminal is established when the
session leader first opens a terminal that is not already the
controlling terminal for a session, unless the O_NOCTTY flag is
specified when calling `open()`. A terminal may be the 
controlling terminal for at most one session.

If a terminal disconnect subsequently occurs, the kernel sends
the controlling process a SIGHUP signal to inform it of this
event.

When a controlling process loses its terminal connection, the 
kernel sends it a SUGHUP signal to inform it of this fact.

Job control permits a shell user to simultaneously execute
multiple commands (jobs), one in the foreground and others in 
the background. Jobs can be stopped and resumed, and moved
between the foreground and background.

# ---------------------------------------------------------------


# Process Priorities and Scheduling

On Linux, the default model for scheduling processes for use of
the CPU is `round-robin time-sharing`. Under this model, each
process in turn is permitted to use the CPU for a brief period
of time, known as a `time slice` or `quantum`.

One process attribute, the `nice value`, allows a process to
indirectly influence the kernel's scheduling algorithm. Each
process has a nice value in the range -20 (high priority) to
+19 (low priority); the default is 0.

The nice value is inherited by a child created via `fork()` and
preserved accross an `exec()`.

The nice value acts as weighting factor that causes the kernel
scheduler to favor processes with higher priorities.

Realtime applications have more stringent requirements of a 
schedular, including the following:

+ A realtime application must provide a guaranted maximum 
  response time for external inputs.
+ A high-priority process should be able to maintain exclusive
  access to the CPU until it completes or voluntarily 
  relinquishes the CPU.
+ A realtime application should be able to control the precise
  order in which its component processes are scheduled.

Linux provides 99 realtime priority level, numbered 1 (lowest)
to 99 (highest), and this range applies in voth realtion 
scheduling policies.

The scheduling policy and priority are inherited by a child 
created via `fork()`, and they are preserved accoss an `exec()`.

When a process is rescheduled to run on a multiprocessor system,
it doesn't necessarily run on the same CPU on which it last 
executed. The Linux kernel tries to ensure `soft` CPU affinity
for a process - wherever possible, the process is rescheduled
to run on the same CPU.

Sometimes, it is desirable to set `hard` CPU affinity for a 
process, so that it explicitly restricted to always running on
one, or a subset, of the avaliable CPUs.

# ---------------------------------------------------------------


# Process Resources

Each process consumes system resources such as memory and CPU
time. The `getrusage()` system call retrieves statistics about
various system resources used by the calling process or all of
its children.

Each process has a set of resource limits that can be used to
restrict the amounts of various system resources that the 
process may consume.

# ---------------------------------------------------------------


# Daemons

A `daemon` is a process with the following characteristics:

+ It is long-lived. Often, a deamon is created at system startup
  and runs until the system is shunt down.
+ It runs in the background and has no controlling terminal.

Many standard daemons run as privileged processes.

To become a daemon, a program performs the following steps:

+ Perform a `fork()`, after which the parent exits and the child
  continues.
+ The child process calls `setsid()` to start a new session and
  free itself of any association with a controlling terminal.
+ If the daemon never opens any terminal devices thereafter, 
  then we don't need to worry about the daemon reacquiring a 
  controlling terminal.
+ Clear the process umask, to ensure that, when the daemon 
  creates files and directories, they have the requested 
  permissions.
+ Change the process's current working directory, typically to
  the root directory (/).
+ Close all open file descriptors that the daemon has inherited
  from its parent.
+ After having closed file descriptors, a daemon normally opens
  /dev/null and uses `dup2()` to make all those descriptors 
  refer to this device.

A daemon typically terminates only when the system shuts down.
The `init` process sends to all of its children during system
shuntdown. If the daemon needs to perform any cleanup before
terminating, it should do so by establishing a handler for this
signal.

Since daemons are long-lived, we must be particularly wary of 
possible memory leaks and file desciptor leaks.

Since a daemon has no controlling terminal, the kernel never 
generates this signal for a daemon. There fore, daemons can use
SIGHUP safely.

The `syslog` facility provides a single, centralized logging 
facility that can be used to log messages by all applications 
on the system.

# ---------------------------------------------------------------


# Writing Secure Privileged Programs

+ One of the best pieces of advice concerning set-user-ID and
  set-group-ID programs is to avoid writing them whenever 
  possible.
+ The programs should always operate with the `least privilege`
  required to accomplish the tasks it is currently performing.
+ When a program reads passwords or other sensitive information,
  it should perform whatever processing is required, and then
  immediately erase the information from memory.
+ The Linux capabilities scheme divides the traditional all-or-
  noting UNIX privilege scheme into distinct units called 
  `capabilities`. A process can independently enable or disable
  individual capabilities.
+ A useful security techniques in certain cases is to establish
  a `chroot` jail to limit the set of directories and files 
  that a program may access.
+ Where appropriate, signals should be caught, blocked, or
  ignored to prevent possible security problems. Furthermore, 
  the design of signal handlers should be as simple as possible,
  in order to reduce the risk of inadvertently creating a race
  condition.
+ The process umask should be set to a value that ensures that
  the process never creates publicly writible files, since
  these could be modified by a malicious user.
+ Privileged programs should avoid making assumptions about the
  input they are given, or the environment in which they are
  running.
+ Buffer overruns allow techniques such as `stack crashing`, 
  whereby a malicious user employs a buffer overrun to place 
  carefully coded bytes into a stack frame in order to force 
  the privileged program to execute arbitrary code. Use 
  `snprintf(), strncpy(), strncat(), etc.` instead of using
  `sprintf(), strcpy(), strcat()` library functions.
+ A privileged program should always check to see whether system
  calls and library functions succeed, and whether they return
  expected values.

# ---------------------------------------------------------------


# Capabilities

In Linux, the superuser privilege is divided into distinct units,
called `capabilities`. Each priviliged operation is associated 
with a particular capability, and a process can perform that
operation only if it has the corresponding capability. Most of
the time, the Linux capability scheme is invisible to us.

A complete implementation of capabilities requires the following:

+ For each privileged operation, the kernel should check 
  whether the process has the relevent capability, rather than
  checking for an effective user ID of 0.
+ The kernel must provide system calls allowing a process's
  capabilities to be retrieved and modified.
+ The kernel must support the notion of attaching capabilities
  to an executable file, so that the process gains the 
  associated capabilities when that file is exceed.

# ---------------------------------------------------------------


# Login Accounting

UNIX systems maintain two data files containing information about
users logging in and out of the system:

+ The `utmp` file maintains a record of users currently logged
  in to the system.
+ The `wtmp` file is an audit train of all user logins and 
  logouts.

# ---------------------------------------------------------------


# Fundamentals of Shared Libraries

Static libraries, also known as `archives`, provides the 
following benefits:

+ We can place a set of commonly used object files into a single
  library file that can be then used to build multiple executable
  without needing to recompile the original source files when
  building each application.
+ Link commands become simpler.

When a program is built by linking against a static library, the
resulting executable file includes copies of all of the object
files that were linked into the program. Thus, when several 
different executables use the same object modules, each 
executable has its own copy of the object modules.

Shared libraries were designed to address these shortcomings. 
The key idea of  a shared library is that a single copy of the 
object modules is shared by all programs requiring the modules.
The object modules are not copied into the linked executable.

# ---------------------------------------------------------------


# Interprocess Communication

UNIX communication and synchronization facilities:

+ `Communication`
+ `Synchronization`
+ `Signals`

We can break the communication facilities into two categories:

+ `Data-transfer facilities`: The key factor distinguishing 
  these facilities is the notion of writing and reading. In order
  to communicate, one process writes data to the IPC facility,
  and another process reads the data.
+ `Shared memory`: Shared memory allows processes to exchange
  information by placing it in a reagion of memory that is shared
  between processes. Shared memory can provide very fast 
  communication.

We can further break data-transfer facilities into the following
subcategories:

+ `Byte stream` --> pipes, FIFOs, datagram sockets.
+ `Message` --> System V message queues, POSIX message queues
+ `Pseudoterminals`

Most modern UNIX systems provide three flovors of shared memory:
System V shared memory, POSIX shared memory, and memory mappings.

A semaphore is the usual synchronization method used with 
shared memory. Data placed in shared memory is visible to all of
the processes that share that memory.

UNIX system provide the following synchronization facilities:

+ `Semaphores`
+ `File locks`
+ `Mutexes and condition variables`

More generally, any of the data-transfer facilities can be used
for synchronization.

Semantically, a FIFO is similar to a pipe. The principal 
difference is that a FIFO has a name within the file system and
is opened in the same was as a regular file. This allows a FIFO
to be used for communication between unrelated processes.

FIFO are also sometimes known as `named pipes`. Opening a FIFO
synchronizes the reading and writing processes.

When a process opens one end of a FIFO, it blocks if the other
end of the FIFO has not yet been opened. Sometimes, it is 
desirable not to block, and for this purpose, the O_NONBLOCK
flag can be specified when calling `open()`.

System V IPC is the label used to refer to three different 
mechanisms for interporcess communication:

+ `Message queues` can be used to pass messages between 
  processes.
+ `Semaphores` permit multiple processes to synchronize their
  actions.
+ `Shared memory` enables multiple processes to share the same
  region of memory.

Message queues allow processes to exchange data in the form of
messages. Although message queues are similar to pipes and FIFOs
in spme respects, they also differ in important ways:

+ Communication via message queues is message-oriented; that is,
  the reader receives whole messages, as written by the writer.
  It is not possible to read part of message.

Client-server application using System V message queues:

+ The use of a single message queue for eaxhanging messages in
  both directions between server and client.
+ THe use of separate message queues for the server and for each
  client.

One common use of a semaphore is to synchronize access to a 
block of shared memory, in order to prevent one process from
accessing the shared memory at the same time as another process
is updating it.

Shared memory allows two or more processes to share the same
region (usually referred to as a `segment`) of physical memory.
Since a shared memory segment becomes part of a process's user-
space memory, no kernel intervention is required for IPC.

Memory mappings can be used for IPC, as well as a range of other
purposes.

The `mmap()` system call creates a new `memory mapping` in the
calling process's virtual address space. A mapping can be of 
two types:

+ `File mapping`
+ `Anonymous mapping`

The memory in one process's mapping may be shared with mappings
in other processes.  When two or more processes share the same 
pages, each process can potentially see the changes to the page
contents made by other processes, depending on whether the
mapping is `private` or `shared`.

The four different types of memory mappings are created and
used as follows:

+ `Private file mapping`
+ `Private anonymous mapping`
+ `Shared file mapping`
+ `Shared anonymous mapping`

The two most common uses of private file mappings are the 
following:

+ To allow multiple processes executing the same program.
+ To map the initialized data segment of an executable or 
  shared library.

When multiple processes create shared mappings of the same file
region, they all share the same physical pages of memory.

Shared file mappings server two purposes: memory-mapped I/O 
and IPC.

An `anonymous mapping` is one that doesn't have a corresponding
file. A MAP_SHARED anonymous mapping allows related processes
(e.g., parent and child) to share a region of memory without 
needing a corresponding mapped file.

System calls that perform operations on a process's virtual 
address space:

+ The `mprotect()` system call changes the protection on a 
  region of virtual memory.
+ The `mlock()` and `mlockall()` system calls lock a region of
  virtual memory into physical memory, thus preventing it from
  being swapped out.
+ The `mincore()` system call allows a process to determine 
  whether the pages in a region of virtual are resident in 
  pyhsical memory.
+ The `madvise()` system call allows a process to advise the 
  kernel about its future patterns of usage of a virtual memory
  region.

The three POSIX IPC mechanisms are the following:

+ `Message queues`
+ `Semaphores`
+ `Shared memory`

The `unlink` call immediately removes the object's name, and
then destroys the object once all processes cease using it.

+ POSIX message queues are reference counted.
+ POSIX messages have an associated priority, and messages are
  always strictly queued (and thus received) in priority order.
+ POSIX message queues provide a feature that allows a process
  to be asyncronously notified when a message is available on a
  queue.

A feature that distinguishes POSIX message queues from their 
System V counterparts is the ability to receive asynchronous
notification of the availability of a message on a previously
empty queue.

If the semaphore currently has a value greater than 0, 
`sem_wait()` returns immediately. If the value of the semaphore
is currently 0, `sem_wait()` blocks until the semaphore value 
rises above 0; at that time, the semaphore is then decremented
and `sem_wait()` returns.

POSIX shared memory allows to us to share a mapped region 
between unrelated processes without needing to create a
corresponding mapped file.

Two different APIs for placing file locks:

+ `flock()`, which places locks on entire files; and
+ `fcntl()`, which places locks on regions of a file.

Using `fcntl()`, we can place a lock on any part of a file, 
ranging from a single byte to the entire file. This form of 
file locking is usually called `record locking`.

# ---------------------------------------------------------------

# SOCKETS

Sockets are a method of IPC that allow data to be exchanged 
between applications, either on the same host or on different
hosts  connected by a network.

Modern operating systems support at least the following domains:

+ `UNIX (AF_UNIX)`
+ `IPv4 (AF_INET)`
+ `IPv6 (AF_INET6)`

Every sockets implementation provides at least two types of 
sockets: stream and datagram.

`Stream sockets` (SOCK_STREAM) provide a reliable, bidirectional,
byte-stream communication channel.

`Datagram sockets` (SOCK_DGRAM) allow data to be exchanged in
the form of messages called `datagrams`.

Passive socket (server): socket() --> bind() --> listen() -->
                         accept() --> read/write() --> close()
Active socket (client):  socket() --> connect() --> read/write()
                         --> close()

# ---------------------------------------------------------------

# TERMINALS

Both a convetional terminal and a terminal emulator have an
associated terminal driver that handles input and output on the
device.

When performing input, the driver is capable of operating in
either of the following modes:

+ `Canonical mode`
+ `Noncanonical mode`

# ---------------------------------------------------------------

# ALTERNATIVE I/O MODELS

Some applications need to able to do one or both of the 
following:

+ Check whether I/O is possible on a file descriptor without
  blocking if it is not possible.
+ Monitor multiple file descriptors to see if I/O is possible
  on any of them.

Nonblocking I/O allows us to periodically check ("poll") 
whether I/O is possible on a file descriptor.

Because of the limitations of both nonblocking I/O and the use
of  multiple threads or processes, one of the following 
alternatives is often preferable:

+ `I/O multiplexing`
+ `Signal-driven I/O`
+ The `epoll` API

Note that none of these techniques performs I/O. They merely
tell us that a file descriptor is ready. Some other system call
must then be used to actually perform the I/O.