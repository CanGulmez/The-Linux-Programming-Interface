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


`-------------------------------------------------------------------------`

 
# PROCESS CREATION

## Overview of `fork()`, `exit()`, `wait()` and `execve()`

For now, we provide an overview of these four system calls and how they
are typically used together:

+ The `fork()` system call allows one process, the parent, to cretae a 
  new process, the child. This is done by making the new child process
  an (almost) exact duplicate the parent: the child obtains copies of
  the parent's stack, data, heap and text segments.
+ The `exit(status)` library function terminates a process, making all
  resources (memory, open file describtors, and so on) used by process
  avaliable for subsequent reallocation by the kernel.
+ The `wait(status)` system call has two purposes. First, if a child of
  this process has not yet terminated by calling `exit()`, then `wait()`
  suspends execution of the process until one of its children has 
  terminated. Second, the termination status of the child is returned
  in the status argument of `wait()`.
+ The `execve(pathname, argv, envp)` system call loads a new program into
  process's memory. The existing program text is discarded, and the 
  stack, data and heap segments are freshly created for the new program.

## Creating a New Process: `fork()`

In many applications, creating multiple processes can be a useful way
of dividing up a task. It also permits greater concurrency (i.e., more
tasks or requests can be handled simultaneously). The key point to 
understanding `fork()` is to realize that afetr it gas completed its
work, two processes exist, and, in each process, execution continues
from where `fork()` returns.

The two processes are executing the same program text, but they have
separate copies of the stack, data, heap segments. After the `fork()`,
each process can modify the variables in its stack, data, and heap
segments without affecting the other process.

It is important to realize that after a `fork()`, it is indeterminate
which of the two processes is next scheduled to use the CPU.

We can combine the use of `fork()` and `wait()` to control the memory 
footprint of a process. The process's memory footprint is the range of
virtual memory pages used by process.

## The `vfork()` System Call

Except where speed is absolutely critical, new programs should avoid the
use of `vfork()` in favor of `fork()`. This is because, when `fork()` is
implemented using copy-on-write semantics, it approaches the speed of 
`vfork()`, and we avoid the eccentric behaviors associated with `vfork()`.

## Race Conditions After `fork()`

After a `fork()`, it is indeterminate which process -the parent or the 
child- next has access to the CPU. Applications that implicitly or
explicitly rely on particular sequence of execution in order to achive
correct results are open to failure due to `race conditions`. 

## Avoiding Race Conditions by Synchronizing with Signals

After a `fork`, if either process needs to wait for the other to complete
an action, then the active process can send a signal after completing the
action; the other process waits for the signal.


`-------------------------------------------------------------------------`


# PROCESS TERMINATION

## Terminating a Process: `_exit()` and `exit()`

The following actions are performed by `exit()`:

+ Exit handlers (functions registered with `atexit()` and `on_exit()`) 
  are called, in reverse order of their registeration.
+ The `stdio` stream buffers are flushed.
+ The `_exit()` system call is invoked, using the value supplied in
  `status`.

## Details of Process Termination

During both normal and abnormal termination of a process, the following
actions occur:

+ Open file descriptors, directory streams, meesage catalog descriptors,
  and conversion descriptors are closed.
+ As a consequence of closing file descriptors, any file locks held by
  this process are released.
+ Any attached System V shared memory segments are detached.
+ All System V semaphore are released.
+ If this is the controlling process for a controlling termimal, and
  SIGHUP signal is sent to each process in the controlling terminal's
  foreground process group, and the terminal is disassociated from
  the session.
+ Any POSIX named semaphores are closed.
+ Any POSIX message queues are closed.
+ All processes in the group are sent a SIGHUP signal followed by a
  SIGCONT signal.
+ Any memory locks established.
+ Any memory mappings established.


`-------------------------------------------------------------------------`


# MONITORING CHILD PROCESSES

In many application designs, a parent process needs to know when one of 
its child processes changes state -when the child terminates or is stopped
by a signal.

## Waiting on a Child Process

In many applications where a parent creates child processes, it is useful
for the parent to be able to monitor the children to find out when and
how they terminate.

The `wait()` system call does the following:

+ If no child of the calling process has yet terminated, the call blocks
  until one of the children terminates. If a child has already terminated
  by the time of the call, `wait()` returns immediately.
+ If `status` is not NULL, information about how the child terminated
  is returned in the integer to which `status` points.
+ The kernel adds the process CPU times and resource usage statistics
  to running totals for all children of this parent process.
+ As its function result, `wait()` returns the process ID of the child
  that has terminated.

The `wait()` system call has a number of limitations, which `waitpid()` was
designed to address:

+ If a parent process has created multiple children, it is not possible
  to `wait()` for the completion of a sprcific child; we can only wait 
  for the next child that terminates.
+ If no child has yet terminated, `wait()` always blocks. Sometimes, it
  would be preferable to perform a nonblocking wait so that if no child
  has yet terminated, we obtain an immediate indication of this fact.
+ Using `wait()`, we can find out only about children that have terminated.
  It is not possible to be notified when a child is stopped by a signal or
  when a stopped child is resumed by delivery of a SIGCONT signal.

The return value and status argument of `waitpid()` are same as for 
`wait()`. The `pid` argument enables the selection of the child to be 
waited:

+ If `pid` is greater then 0, wait for the child whose process ID equals
  `pid`.
+ If `pid` equals 0, wait for any child in the `same process groups as a`
  `caller` (parent).
+ If `pid` is less then -1, wait for any child whose `process group` 
  identifier equals the absolute value of `pid`.
+ If `pid` equals -1, wait for any child. The call `wait(status)` is
  equivalent to the call `waitpid(-1, status, 0)`.

The `<sys/wait.h>` header file defines a standard set of macros that can
be used to dissect a wait status value. When applied to a status value 
returned by `wait()` or `waitpid()`, only one of the macros in the list
below will return true.

+ WIFEXITED(status)
+ WIFSIGNALED(status)
+ WIFSTOPPED(status)
+ WIFCONTINUED(status)

## Orphans and Zombies

The lifetimes of parent and child processes are usually not the same 
-either the parent outlives the child or vice verse. This raises two
questions:

+ Who becomes the parent of an `orphaned` child? The orphaned child is
  adopted by `init`, the encestor of all processes, whose process ID is 1.
  After a child's parent terminates, a call to `getppid()` will return 
  the value 1.
+ What happens to a child that terminates before its parent has had a
  change to perform a `wait()`? Although the child has finished its work,
  the parent should still be permitted to perform a `wait()` at some 
  later time to determine how the child terminated. The kernel deals
  with this situation by turning the child into a `zombie`. This means
  that most of the resources held by the child are released back to the
  system to be reused by other processes.

A zombie process can't be killed by a signal, not even the (silver bullet)
SIGKILL. This ensures that the parent can always evetually perform a 
`wait()`.

When the parent does perform a `wait()`, the kernel removes the zombies,
since the last remanining information about the child is no longer 
required. On the other hand, if the parent terminates without doing a
`wait()`, then the `init` process adopts the child and automatically 
performs a `wait()`, thus removing the zombie process from the system.

If a parent creates a child, but fails to perform a `wait()`, then an 
entry for the zombie child will be maintained indefinitely in the kernel's
process table. If a large number of such zombie children are created, they
will eventually fill the kernel process table, preventing the creation of
new processes. Since the zombies can't be killed by a signal, the only
way to remove them from the system is to kill their parent, at which time
the zombies are adopted and waited on by `init`, and consequently removed
from the system.

## The `SIGCHLD` Signal

The termination of a child process is an event that occurs asynchronously.
A parent can't predict when one of its child will terminate.


`-------------------------------------------------------------------------`


# PROCESS EXECUTION

## Executing a New Program: `execve()`

The `execve()` system call loads a new program into a process's memory. 
During this operation, the old program is discarded, and the process's
stack, data, and heap are replaced by those of the new program. After an
`execve`, the process ID of the process remains the same, because the 
same process continues to exist.

## Interpreter Scripts

An `interpreter` is a program that reads commands in text form and 
executes them.

## Executing a Shell Command: `system()`

The `system()` function allows the calling program to execute an 
arbitrary shell command. The `system()` function creates a child process
that invokes a shell to execute `command`. The principal advantages of 
`system()` are simplicity and convenience:

+ We don't need to handle the details of calling `fork()`, `exec()`, 
  `wait()`, and `exit()`.
+ Error and signal handling are performed by `system()` on out behalf.
+ Because `system()` uses the shell to execute `command`, all of the 
  usual shell processing, substitutions, and redirections are performed
  on `command` before it is executed.


`-------------------------------------------------------------------------`


# PROCESS CREATION AND PROGRAM EXECUTION IN MORE DETAIL

# Process Accounting

When process accounting is enabled, the kernel writes an accounting record
to the system-wide process accounting file as each process terminates. 
This accounting record contains various information maintaned by the kernel
about process, including its termination status and how much CPU time is
consumed.


`-------------------------------------------------------------------------`


# PROCESS GROUPS, SESSIONS, AND JOB CONTROL

## Overview

A `process group` is a set of one or more processes sharing the same 
`process group identifier` (PGID). A process group has a `process group`
`leader`, which is the process that creates the group and whose process
ID becomes the process group ID of the group.

A `session` is a collection of process groups. A process's session 
membership is determined by its `session identifier` (SID), which, like 
the process group ID. A `session leader` is th process that creates a new 
session and whose process ID becomes the session ID. All of the processes
in a session share a single `controlling terminal`. The controlling 
terminal is established when the session leader first opens a terminal
device. A terminal may be the controlling terminal of at most one session.

At any point in time, one of the process groups in a session  is the 
`foreground process group` for the terminal, and the others are 
`background process group`. Only processes in the foreground process 
group can read input from the controlling terminal. When the use types
one of the signal-generating terminal characters on the controlling 
terminal, a signal is sent to all members of the foreground process group.


`-------------------------------------------------------------------------`


# PROCESS PRIORITIES AND SCHEDULING

## Process Priorities (Nice Values)

On linux, as with most other UNIX implementations, the default model for
scheduling processes for use of the CPU is `round-robin time-sharing`. 
Under this model, each process in tuen is permitted to use the CPU for a
brief period of time, known as a `slice` or `quantum`.

Under the round-robin time-sharing algorithm, processes can't execise 
direct control over when and for how long they will be able to use the
CPU. By default, each process in turn receives use of the CPU until its
time slice runs out or voluntarily gives up the CPU (for example, by
putting itself to sleep or performing a disk read).

Howover, one process attribute, the `nice value`, allows a process to
indirectly influence the kernel's scheduling algorithm. Each process has
a nice value in the range -20 (high priority) to +19 (low priority); the
default is 0.

## Overview of Realtime Process Scheduling

The standard kernel scheduling algorithm usually provides adequate 
performance and responsiveness for the mixture of interactive and 
background processes typically run on a system. However, realtime
applications have more strigent requirements of  a scheduler, including
the following:

+ A realtime application must provide a guaranteed maximum response time
  for external inputs.
+ A high-priority process should be able to maintain exclusive access to
  CPU until it completes or voluntarily relinquishes the CPU.
+ A realtime application should be able to the precise order in which its
  component processes are scheduled.

### The `SCHED_RR` Policy

Under the `SCHED_RR` (round-robin) policy, processes of equal priority are
executed in a round-robin time-sharing fasion. A process receives a fixed-
lenght time slice each time it uses the CPU.

### The `SCHED_FIFO` Policy

The `SCHED_FIFO` (first-in, first-out) policy is similar to the 
`SCHED_FIFO` process gains access to the CPU.

## Realtime Process Scheduling API

We now look at the various system calls constituting the realtime process
scheduling API. These system calls allow us to control process secheduling
policies and priorities.

## CPU Affinity

When a process is rescheduled to run on a multiprocessor system, it doesn't
necassarily run on the same CPU on which it last executed. The usual reason
it may run on another CPU is that original CPU is already busy.


`-------------------------------------------------------------------------`


# PROCESS RESOURCES

Each process consumes system resources such as memory and CPU time. 

## Details of Specific Resource Limits

In this section, we provide details on each of the resource limits 
avaliable on Linux, noting those that are Linux-specific:

+ RLIMIT_AS:     Specify the maximum size for the process's virtual memory
                 (address space), in bytes.
+ RLIMIT_CORE:   Specify the maximum size, in bytes, for core dump file.
+ RLIMIT_CPU:    Specify the maximum number of seconds of CPU time that
                 can be used by the process.
+ RLIMIT_DATA:   Specify the maximum size, in bytes, of the process's 
                 data segment.
+ RLIMIT_FSIZE:  Specify the maximum size of files that the process may
                 create, in bytes.
+ RLIMIT_MEMLOCK:Specify the maximum number of bytes of virtual memory
                 that a process may lock into physical memory.
+ RLIMIT_MSGQUEUE
+ RLIMIT_NICE
+ RLIMIT_NOFILE: Specify a number one greater than the maximum file 
                 descriptor number that a process may allocate.
+ RLIMIT_NPROC
+ RLIMIT_RSS:    Specify the maximum number of pages in the process's 
                 resident set.
+ RLIMIT_RTPRIO: Specify a ceiling on the realtime priority that may be
                 set for this process.
+ RLIMIT_RTTIME
+ RLIMIT_SIGPENDING
+ RLIMIT_STACK:  Specify the maximum size of the process stack, in bytes.


`-------------------------------------------------------------------------`


# DAEMONS

## Overview

A `daemons` is a process with the following characteristics:

+ It is long-lived. Often, a daemon is created at system startup and runs
  until the system is shut down.
+ It runs in the background and has no controlling terminal.

Many standard daemons run as privileged process (i.e., effective user
ID of 0).

## Creating a Daemon

To become a daemon, a program performs the following steps:

+ Perform a `fork()`, after which the parent exits and the child 
  continues. (As a consequence, the daemon becomes a child of the `init`
  process.)
+ The child process calls `setsid()` to start a new session and free 
  itself of any associated with a controlling terminal.
+ If the daemon never opens any terminal devices thereafter, then we don't
  need to worry about the daemon reacquiring a controlling terminal. If
  the daemon might later open a terminal device, then we must take steps
  to ensure that the device does not become the controlling terminal.
+ Clear the process unmask to ensure that when the daemon creates files 
  and directories, they have the requested permissions.
+ Change the process's current working directory, typically to the root
  directory (/).
+ Close all open file descriptors that the daemon has inherited from its
  parent. And as usual, we should close unused open file descriptors 
  because file descriptors are a finite resource.
+ After having closed file descriptors 0, 1, and 2, a daemon normally 
  opens `/dev/null` and uses `dup()` to make all those descriptors refer
  to this device.

## Using `SIGHUP` to Reinitialize a Daemon

The fact that many daemons should run continuously presents a couple
of programming hurdles:

+ Typically, a daemon reads operational parameters from a associated 
  configuration file on startup. 
+ Some daemons produces log files. If the daemon never closes the log
  file, then it may grop endlessly.

The solution to both of these problems is to have the daemon establish
a handler for SIGHUP. SIGHUP is generated for the controlling process on
disconnection and kernel never generates this signal for daemon.


`-------------------------------------------------------------------------`


# WRITING SECURE PRIVILEGED PROGRAMS

Privileged programs have access to features and resources that are not 
avaliable to ordinary users. A program can run with privileges by two 
general means:

+ The program was started under a privileged user ID. Many daemons and
  network servers, which are typically run as `root`, fall into this
  category.
+ The program has its set-user-ID and set-group-ID permission bit set.

## Is a Set-User-ID or Set-Group-ID Program Required?

One of the best pieces of advice concerning set-user-ID and set-group-ID
programs is to avoid writing them whenever possible. If there is an
alternative way of performing task that doesn't involve givening a program
privilege, we should generally employ that elternative, since it eliminates
the possibility of a security compromise.

## Operate with Least Privilege

A set-user-ID program typically requires privileges only to perform certain
operations. While this program is performing other work, it should disable
these privileges.


`-------------------------------------------------------------------------`


# CAPABILITIES

## Rationale for Capabilities

The traditional UNIX privilege scheme divides processes into two 
categories: those whose effective user ID is 0 (superuser), which bypass
all privilege checks, and all other process, which are subject to privilege
checking according to their user and group IDs.

The Linux capability scheme refines the handling of this problem. Rather
than using a single privilege (i.e., effective user ID of 0) when 
performing security checks in the kernel, the superuser privilege is 
divided into distinct units, called `capabilities`.

## Process and File Capabilities

Each process has three associated capability sets -termed `permitted`, 
`effective`, and `inheritable`.

## The Modern Capabilities Implementation

A complete implementation of capabilities requires the following:

+ For each privileged operation, the kernel should check whether the 
  process has the relevent capability, rather than checking for an 
  effective (or file system) user ID of 0.
+ The kernel must provide system calls allowing a process's capabilities
  to be retrieved and modified.
+ The kernel must support the notion of attaching capabilities to an
  executable file, so that the process gains the associated capabilties
  when that file is execed.


`-------------------------------------------------------------------------`


# LOGIN ACCOUNTING

Login accounting is concerned with recording which users are currently
logged in to the system, and recording past logins and logouts. This 
chapter looks at the login accounting files and the library functions 
used to retrieve and update the information theny contain.

## Overview of the `utmp` and  `wtmp` Files

+ The `utmp` file maintains a record of users currently logged in to the
  system.
+ The `wtmp` is an audit train of all user logins and logouts.

