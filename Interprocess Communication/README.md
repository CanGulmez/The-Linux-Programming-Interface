# INTERPROCESS COMMUNICATION OVERVIEW

## A Taxanomy of IPC Facilities

UNIX communication and synchronization facilities can divide them
into three broad functional categories:

+ `Communication`: These facilities are concerned with exchanging 
  data between processes.
+ `Synchronization`: These facilities are concered with syncronizing
  the actions of processes or threads.
+ `Signals`: Although signals are intended primarily for other 
  purposes, they can be used as a synchronization technique in 
  certain circumstances.

Although some of these facilities are concerned with synchronization,
the general term `interprocess communication` (IPS) is often used 
to describe them all.

## Communication Facilities

We can break the communication facilities into two categories:

+ `Data-transfer facilities`: The key factor distinguishing these
  facilities is the notion of writing and reading. In order to 
  communicate, one process writes data to the IPC facilitiy, and
  another process reads the data.
+ `Shared memory`: Shared memory allows processes to exchange 
  information by placing it in a region of memory that is shared 
  between the processes. Shared memory can provide very fact
  communication.

### Data transfer

We can further break data-transfer facilities into the following 
subcategories:

+ `Byte stream`: The data exchanged via pipes, FIFOs, and datagram
  socket undelimited byte stream.
+ `Message`: The data excahnged via System V and POSIX message queue.
+ `Pseudoterminals`: A pseudoterminal is a communication facility
  intended for use in specialized situations.

A few general features distinguish data-transfer facilities from 
shared memory.

### Shared memory

Most modern UNIX systems provide three flavors of shared memory: 
System V shared memory, POSIX shared memory, and memory mappings.

## Synchronization Facilities

UNIX systems provide the following synchronization facilities:

+ `Semaphores`: A semaphore is a kernel-maintained integer whose 
  value is never permitted to fall below 0. A process can decrease 
  or increase the value of a semaphore. If an attempt is made to
  decrease the value of the semaphore below 0, then the kernel blocks
  the operation until the semaphore's value increases to a level that
  permits the operation to be performed.
+ `File locks`: File locks are a synchronization method explicitly 
  designed to coordinate the actions of multiple processes operatiing
  on the same file.
+ `Mutex and condition variables`: These synchronization facilities
  are normally used with POSIX threads.

## Comparing IPC Facilities

We begin by summarizing the differences between data-transfer 
facilities and shared memory:

+ Data-transfer facilities involve read and write operations, with
  transferred data being comsumable by just one reader process.
+ Other application designs more naturally suit a shared-memory 
  model. Shared memory allows one process to make visible to any 
  number of other processes sharing the same memory region.

With respect to the various data-transfer facilities, the following
points are worth noting:

+ Some data-transfer facilities transfer data as a byte stream,
  (pipes, FIFOs, and stream sockets); others are message-oriented
  (message queues and datagram sockets).
+ A distinctive feature of System V and POSIX message queues, 
  compared with other data-transfer facilities, is the ability to
  assign a numeric type or priority to a message, so that messages
  can be delivered in a different order from that in which they
  were sent.
+ Pipes, FIFOs, and sockets are implemented using file descriptors.
  These IPC facilities all support a range of alternative I/O models.
+ POSIX message queues provide a notification facility that can a
  signal to a process.
+ UNIX domain sockets provide a feature that allows a file descriptor
  to be passed from one process to another.
+ UDP sockets allow a sender to broadcast or multicast a message to
  multiple recipients.


`--------------------------------------------------------------------`


# PIPES AND FIFOS

Pipes can be used to pass data between related process. FIFOs are a 
variation on the pipe concept. The important difference is that FIFOs
can be used for communication between any processes.

We look at how signals could be used to synchronize the actions of
parent and child processes in order to avoid race conditions. Pipes
can be used to achive a similar result.

A common use for pipes is to execute a shell command and either read
its output or send it some input. The `popen()` and `pclose()` 
functions are provided to simplify this task.

Semantically, a FIFO is similar to a pipe. The principal difference
is that a FIFO has a name within the file system and is opened in the
same way as a regular file. This allows a FIFO to be used for 
communication between unrelated processes.

Once a FIFO has been opended, we use the same I/O system calls are 
used with pipes and other files.


`--------------------------------------------------------------------`


# Introduction to System V IPS

System V IPC is the label used to refer to three different mechanisms
for interprocess communication:

+ `Message queues` can be used to pass messages between processes.
+ `Semaphores` permit multiple process to synchronize their actions.
+ `Shared memory` enables multiple processes to share the same region
  (called `segment`) of memory

## API Overview

Each System V IPC mechanism has an associated `get` system call, which
is analogous to the `open()` system call used for files. Given an 
integer `key` (anolagous to a filenmane), the `get` call either:

+ creates a new IPC object with the given key and returns a unique 
  identifier for that object; or
+ returns the identifier of an existing IPC object with the given key.

We'll (loosely) term the second use `opening` an existing IPC object.
In this case, all that the `get` call is doing is converting one 
number (the key) into another number (the identifier).

An IPC `identifier` is analogous to a file descriptor in that it is 
used in all subsequent system calls to refer to the IPC object.

The `ctl` system call for each System V IPC mechanism performs a range
of `control operations` for the object.

For message queues and semaphores, delection of the IPC object is
immediate, and any information contained within the object is 
destroyed. Deletion of shared memoy objects occurs differently. 
Following the `schctl(id, IPC_RMID, NULL)` call, the shared memory 
segment is removed only after all processes using the segment detach
it.

System V IPC keys are integer values represented using the data type
`key_t`. The IPC `get` calls translate a key into the corresponding 
integer IPC identifier. These calls guarantee that if we create a new
IPC object, then that object will have a unique identifier, and that
if we specify the key of an existing object, then we'll always obtain
the (same) identifier for that object. To provide a unique key, using
either IPC_PRIVATE or `fork()` is the usual technique.

For each IPC mechanism, the kernel maintains an associated `ipc_ids` 
structure that records various global information about all instances
of the IPC mechanism. This information includes a dynamically sized
array of pointers, `entries`, to the associated data struture for each
object instance.


`--------------------------------------------------------------------`


# SYSTEM V MESSAGE QUEUES

Message queues allow processes to exchange data in forn of messages.
Although message queues are similar pipe and FIFOs in some respects, 
they also differ in important ways:

+ The handle used to refer  to a message queue is the identifier 
  returned by a call to `msgget()`. These identifiers are not the 
  same as the file desriptors used for most other forms of I/Os.
+ Communication via message queue is message-oriented; that is, the 
  reader receives whole messages, as written by writer.
+ In addition to containing data, each message has an integer `type`.
  Messages can be retrieved from a queue in first-in, first-out order
  on retrieved by type.


`--------------------------------------------------------------------`


# SYSTEM V SEMAPHORES

System V semaphores are not used to transfer data between processes.
Instead, they allow processes to synchronize their actions. One common
use of a semaphore is to synchronize access to a block of shared 
memory, in order to prevent one process from accessing the shared 
memory at the same time as another process is updating it.

A semaphore is a kernel-maintained integer whose value is restricted
to being greater than or equal to 0.

In terms of controlling the actions of a process, a semaphore has no
meaning in and itself. Its meaning is determined only by the 
associations given to it by the processes using the semaphore. 
Typlically, processes agree on a convention that associates a 
semaphore with a shared memory, such as a region of shared memory.

## Overview

The general steps for using a System V semaphores are the following:

+ Create or open a semaphore set using `semget()`
+ Initialize the semaphores in the set using the `semctl()` SETVAL or
  SETALL operations.
+ Perform operations on semaphore values using `semop()`.
+ When all processes have finished using the semaphores set, remove
  the set using the `semctl()` IPC_RMID operation.


`--------------------------------------------------------------------`


# SYSTEM V SHARED MEMORY

Shared memory allows two or more processes to share the same region
(usually referred to as a `segment`) of physical memory. Since a 
shared memory segment becomes part of a process's user-space memory,
no kernel intervention is required for IPC. All that is required is
that one process copies data inro the shared memory; that data is
immediately avaliable to all other process sharing the same segment.
This provides fast IPC by comparision with techniques such as pipes
or message queues. 

System V semaphores are a natual method for such synchronization.
Other methods, such as POSIX semahores and file locks are also 
possible.

## Overview

In order to use a shared memory segment, we typically perform the
following steps:

+ Call `shmget()` to create a new shared memory segment or obtain of
  existing segment. This call returns a shared memory identifier for
  use in later calls.
+ Use `shmat()` to `attach` the shared memory segment; that is, make
  the segment part of the virtual memory of the calling process.
+ At this point, the shared memory segment can be treated just like
  any other memory avaliable to the program.
+ Call `shmdt()` to detach the shared memory segment. After this call,
  the process can no longer refer to the shared memory. This step is
  optional, and happens automatically on process termination.
+ Call `shmctl()` to delete the shared memory segment.The segment will
  be destroyed only after all currently attached processes have 
  detached it. Only one process needs to perform this step.


`--------------------------------------------------------------------`
 

# MEMORY MAPPINGS

Memory mappings can be used for IPC, as well as a range of other
purposes. 

## Overview

The `mmap()` system call creates a new `memory mapping` in the calling
process's virtual address space. A mapping can be of two types:

+ `File mapping`: A file mapping maps a region of a file directly
  into the calling process's virtual memory.
+ `Anonymous mapping`: An anonymous mapping doesn't have a 
  corresponding file. Instead, the pages of the mpping are initialized
  to 0.

The memory in one process's mapping may be shared with mappings in
other processes. This can occur in two ways:

+ When two processes map the same region of a file.
+ A child process created by `fork()` inherits copies of its parent's
  mappings.

When two or more processes share the same pages, each process can
potentially see the changes to the page contents made by other
processes, depending on whether the mapping is `private` (MAP_PRIVATE)
or `shared` (MAP_SHARED).

The four different types of memory mappings are created and used as
follows:

+ `Private file mapping`
+ `Private anonymous mapping`
+ `Shared file mapping`
+ `Shared anonymous mapping`

## File Mappings

To create a file mapping, we perform the following steps:

1. Obtain a descriptor for a file, typically via a call to `open()`.
2. Pass that file descriptor as the `fd` argument in a call to 
   `mmap()`.
   
The file reffered to by the descriptor `fd` must have been opened with
permissions appropriate for the values specified in `prot` and `flags`
. In particular, the file must always be opened for reading, and, if
PROT_WRITE and MAP_SHARED are specified in `flags`, then the file must
be opened for reading and writing.

## Anonymous Mappings

An `anonymous mapping` is one that doesn't have a corresponding file.

### MAP_ANONYMOUS and /dev/zero

On Linux, there are two different, equivalent methods of creating an
anonymous mapping with `mmap()`.

+ Specify MAP_ANONYMOUS in `flags` and specify `fd` as -1.
+ Open the `/dev/zero` device file and pass the resulting file 
  descriptor to mmap().

With both the MAP_ANONYMOUS and the `/dev/zero` techniques, the bytes
of the resulting mapping are initialized to 0. the `offset` argument
is ignored.

## Remapping a Maped Region: `mremap()`

On most UNIX implementations, once a mapping has been created, its
location and size can't be changed. Howover, Linux provides the 
(nonportable) `mremap()` system call, which permits such changes.


`--------------------------------------------------------------------`


# VIRTUAL MEMORY OPERATIONS

This chapter looks at various system calls that perform operations on
a process's virtual address space:

+ The `mprotect()` system call changes the protection on a region of
  virtual memory.
+ The `mlock()` and `mlockall()` system calls lock a region of virtual
  memory into physical memory, thus preventing it from being swapped
  out.
+ The `mincore()` system call allows a process to determine whether
  the pages in a region of virtual memory are resident in physical
  memory.
+ The `madvise()` system call allows a process to advise the kernel
  about its future patterns of usage of a virtual memory region.


`--------------------------------------------------------------------`


# INTRODUCTION TO POSIX IPC

The three POSIX IPC mechanisms are the following:

+ `Message queue`
+ `Semaphores`
+ `Shared memory`

The details of three POSIX IPC mechanisms are same as System V.

## Comparison of System V IPC and POSIX IPC

POSIX IPC has the following general advantages when compared to 
System V IPC:

+ The POSIX IPC interface is simpler than the System V IPC interface.
+ The POSIX IPC nodel is more consistent with the traditional UNIX
  file model.
+ POSIX IPC objects are reference counted.

POSIX IPC is less portable than System V IPC in the following 
respects:

+ System V IPC is specified in SUSv3 and supported on nearly evey
  UNIX implementation. By constant, each of the POSIX IPC mechanisms
  is an optional component in SUSv3.
+ Despite the SUSv3 specification for POSIX IPC object names, the 
  various implementations follow different conventions for naming IPC
  objects.
+ Various details of POSIX IPC are not specified in SUSv3.


`--------------------------------------------------------------------`


# POSIX MESSAGE QUEUES

POSIX message queues allow processes to exchange data in the form of
messages. POSIX message queues are similar to their System V 
counterparts. Howover, there are also some notable differences:

+ POSIX message queues are referenced counted.
+ POSIX messages have an associated priority, and messages are always
  strictly queued (and thus received) in priority order.
+ POSIX message queues provide a feature that allows a process to be
  asynchronously notified when a message is avaliable on a queue.

## Relationship Between Descriptors and Message Queues

The relationship between message queue descriptor and an open message
queue is analogous to the relationship between a file desriptor and
an open file.


`--------------------------------------------------------------------`


# POSIX SEMAPHORES

POSIX semaphores allow processes and threads to synchronize access to
shared resources.

## Overview

SUSv3 specifies two types of POSIX semaphores:

+ `Named semaphores`: This type of semaphore has a name. By calling
  `sem_open()` with the same name, unrelated processes can access the
  same semaphore
+ `Unnamed semaphores`: This type of semaphore doesn't have a name:
  instead, it resides at an agreed-upon location in memory.

## Named Semaphores

To work with a named semaphore, we employ the following functions:

+ The `sem_open()` function opens or create a semaphore, initializes
  the semaphore if it is created by the call.
+ The `sem_post()` and `sem_wait()` functions respectively increment
  and decrement a semaphore's value.
+ The `sem_getvalue()` function retrieves a semaphore's current value.
+ The `sem_close()` function removes the calling process's association
  with a semaphore that it previously opened.
+ The `sem_unlink()` function removes a semaphore name and marks the
  semaphore for delation when all proocesses have closed it.

## Unnmaed Semaphores

Unnamed semaphores (also known as `memory-based semaphores`) are
variables of type `sem_t` that are stored in memory allocated by the
application.The semaphore is made avaliable to the processes or t
hreads that use it by placing it in an area of memory that they share.

Operations on unnamed semaphores use the same functions that are used
to operate on named semaphores. In addition, two further functions
are required:

+ The `sem_init()` function initializes a semaphore.
+ The `sem_destroy()` function destroys a semaphore.


`--------------------------------------------------------------------`


# POSIX SHARED MEMORY

POSIX shared memory allows to us to share a mapped region between 
unrelated processes without needing to create a corresponding mapped
file.

To use a POSIX shared memory object, we peroform two steps:

+ Use the `shm_open()` function to open an object with a specified 
  name. The `shm_open()` function is analogous to the `open()` system
  call. It either creates a new shared memory object or opens an
  existing object. As its function result, `shm_open()` returns a
  file descriptor referring to the object.
+ Pass the file descriptor obtained in the previous step in a call to
  `mmap()` that specifies MAP_SHARED in the `flags` argument.


`--------------------------------------------------------------------`


# FILE LOCKING

A frequent application requirement is to read data from a file, make
some changes to that data, and then write it back to the file. As 
long as just one process at a time ever uses a file in this way, then
there are no problems. Howover, problems can arise if multiple 
processes are simultaneously updating a file.

In this chapter, we describe two different APIs for placing file 
locks:

+ `flock()`, which places locks on entire files; and
+ `fcntl()`, whixh places locks on regions of a file.

The general method of using `flock()` and `fcntl()` is as follow:

1. Place a lock on the file.
2. Perform file I/O.
3. Unlock the file so that another process can lock it.

Although file locking is normally used in conjunction with file I/O,
we can also use it as a more general synchronization technique.

