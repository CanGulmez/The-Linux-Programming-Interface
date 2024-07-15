# Processes

## Process and Programs

A `process` is an instance of an executing program. A `program` is a
file containing a range of information that describes how to construct
a process at run time. This information includes the following:

+ `Binary format identification`
+ `Machine-language instructions`
+ `Program entry-point address`
+ `Data`
+ `Symbol and relocation tables`
+ `Shared-library and dynamic-linking information`
+ `Other information`

## Process ID and Parent Process ID

A process is an abstract entity, defined by the kernel, to which 
system resources are allocated in order to execute a program.

Each process has a process ID (PID), a positive integer that uniquely
identifies the process on the system. Each process has a parent - the
process that created it. If a child process becomes orphaned because
its "birth" parent terminates, then the child is adopted by the `init`
process.

## Memory Layout of a Process

The memory allocated to each process is composed of a number of parts,
usually reffered to as `segments`. These segments are as follows:

+ The `text segment` contains the machine-language instructions of the
  program run by the process.
+ The `initialized data segment` contains global and static variables
  that are explicitly initialized.
+ The `uninitialized data segment` contains global and static 
  variables that are explicitly uninitialized.
+ The `stack` is a dynamically growing and shrinking segment 
  containing stack frames.
+ The `heap` is an area from which memory (for variables) can be 
  dynamically allocated at run time. The top end of the heap is called
  the `program break`.

Typical memory layout of a process on Linux/x86-32:

              |------------------------------|
              |            Kernel            |
   0xC0000000 |------------------------------|
              |         argv, environ        |
              |------------------------------|
              |            Stack             |
              |      (grows downwards)       |
 Top of stack |------------------------------|
              |                              |
              |     (unallocated memory)     |
              |                              |
Program break |------------------------------|
              |             Heap             |
              |        (grows upwards)       |
              |------------------------------|  &end
              |   Uninitialized data (bss)   |
              |------------------------------|  &edata
              |       Initialized data       |
              |------------------------------|  &text
              |      Text (program code)     |
   0x08048000 |------------------------------|
              |                              |
   0x00000000 |------------------------------|

## Virtual Memory Management

Linux employs a technique known as `virtual memory management`. The 
aim of this technique is to make efficient use of the both CPU and
RAM(physical memory) by exploiting a property that is typical of most
programs: `locaity of reference`:

+ `Spatial locality` is the tendency of a program to reference memory
  addresses that are near those that were recently accessed.
+ `Temporal locality` is the tendency of a program to access the same
  memory-addresses in the near future that it accessed in the recent
  pas.

A virtual memory scheme splits the memory used by each program into
small, fixed-size units called `pages`. Correspondingly, RAM is divided
into a serial of `pages frames` of the same size. At any one time, only
some of the pages of the program need to be resident in physical 
memory frames; these pages form the so-called `resident set`. When a 
process references a page that is not currently resident in physical
memory, a `page fault` occurs.

In order to support this organization, the kernel maintains a `page`
`table` for each process. The page table decribes the location of each
page in the process's `virtual address space`.

## The Stack and Stack Frames

A special purpose register, the `stack pointer`, tracks the current
top of the stack. Each time a function is called, an additional frame 
is allocated on the stack, and this frame is removed when the function
returns.

Sometimes, the term `user stack` is used to distinguish the stack we
describe here from the `kernel stack`. The kernel stack is per-process
memory region maintained in kernel memory that is used as the stack
for execution of the functions called internally during the execution
of the system call.

Each (user)stack frame contains the following:

+ Function arguments and local variables
+ Call linkage information

## Command-Line Arguments (`argc`, `argv`)

## Environment List

Each process has an associated array of strings called `environment`
`list`, or simply the `environement`. The names in the list are
reffered to as `environment variables`. When a new process is created,
it inherits a copy of its parent's environment.

## Performing a Nonlocal Goto: `setjmp()` and `longjmp()`

The `setjmp()` and `longjmp()` library functions are used to perform
a `nonlocal goto`. Calling `setjmp()` establishes a target for a later
jump performed by `longjmp()` call occured.


`---------------------------------------------------------------------`


# Users and Groups

Every user has a unique login name and an associated number user 
identifier (UID). Users can belong to one or more groups. Each group
also has a unique name and a group identifier (GID).

## The Password File: */etc/passwd*

The system `password file`, *etc/passwd*, contains one line for each 
user account on the system. Each line is composed of seven fields 
separated by colon:

+ `Login name`
+ `Encrypted password`
+ `User ID` (UID)
+ `Group ID` (GID)
+ `Comment`
+ `Home directory`
+ `Login shell`

## The Shadow Password File: */etc/shadow*

Historically, UNIX systems maintained all user information, including
the encrypted password, in */etc/passwd*. This presented a security 
problem. Since various unprivileged system utilities needed to have 
read access to other information in the password file, it had to be 
made readable to all users. The `shadow password file`, */etc/shadow*, 
was devised as a method of preventing such attacks.

## The Group File: */etc/group*

The `group file`, */etc/group*, contains one line for each group in the
system. Each line consists of four colon-seperated fields. In order:

+ `Group name`
+ `Encrypted password`
+ `Group ID` (GID)
+ `User list`

## Password Encryption and User Authentication

Some applications require that users authenticate themselves. These 
applications must validate a username and password in the same way 
that the standard `login` program does.


`---------------------------------------------------------------------`


# Process Credentials

Every process has a set of associated numeric user and group 
identifiers, sometimes, these are reffered to as process credentials. 
These identifiers are as follows:

+ real user ID and group ID;
+ effective user ID and group ID;
+ saved set-user-ID and saved set-group-ID;
+ file-system user ID and group ID (Linux-specific); and
+ supplementary groups IDs.

## Real User ID and Real Group ID

The real user ID and group ID identify the use user and group to which
the process belongs.

## Effective User ID and Effective Group ID

On most UNIX implementations, the effective user ID and group ID, in
conjunction with the supplementary groups IDs, are used to determine the
permissions granted to a process when it tries to perform various operation
(i.e., system calls). The effective user ID is also used by the kernel to 
determine whether one process can send a signal to another.

A process whose effective use ID is 0 has all of the privileges of the
superuser. Such a process is reffered to as a `priviliged process`. 
Normally, the effective user and group IDs have the same values as the
corresponding real IDs.

## Set-User-ID and Set-Group-ID Programs

A set-user-ID program allows a process to gain privileges it would not
normallu have, by setting the process's effective user ID to the same 
value as the user ID of the executable file. A set-group-ID program 
performs the analogous task for the process's effective group ID.

An executable file has two special permission bits: the set-user-ID and
set-group-ID bits. These permission bits are set using the `chmod` command.

## Saved Set-User-ID and Saved Set-Group-ID

The saved set-user-ID and saved set-group-ID are designed for use with 
set-user-ID and set-group-ID programs.

## File-System User ID and File-System Group ID

On Linux, it is the file-system user and group IDs, rather than the 
effective user and group IDs, that are used to determine permissions when
performing file-system operations. Normally, the file-system user and 
group IDs have the same value as the corresponding effective IDs.

## Supplementary Group IDs

The supplementary group IDs are a set of additional groups to which a
process belongs.


`-------------------------------------------------------------------------`


# System Limits and Options

Each UNIX implementation sets limits on various system features and
resources. The C programming language standards ans SUSv3 provide two
principal avenues for an application to obtain such information:

+ Some limits and options can be determined at compile time. Such limits
  can be recorded in header files.
+ Other limits and options may vary at run time. For such cases, SUSv3
  defines three functions - `sysconf()`, `pathconf()`, and `fpathconf()`.


`-------------------------------------------------------------------------`


# System and Process Information

## The `/proc` File System

In order to provide easier access to kernel information, many modern UNIX
implementations provide a `/proc` virtual file system. The file system 
resides under the `/proc` directory and contains various files that expose
kernel information, allowing processes to conveniently read information,
and change it in some cases, using normal file I/O system calls. The 
`/proc` file system is said to be virtual because the files and 
subdirectories that it contains don't reside on a disk. Instead, the 
kernel creates them "on the fly" as processes access them:

+ `/proc`            --> Various system information
+ `/proc/net`        --> Status information about networking and sockets
+ `/proc/sys/fs`     --> Settings related to file systems
+ `/proc/sys/kernel` --> Various general kernel settings
+ `/proc/sys/net`    --> Networking and sockets settings
+ `/proc/sys/vm`     --> Memory-management settings
+ `/proc/sysvipc`    --> Information about System V IPC objects

