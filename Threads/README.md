# THREADS: INTRODUCTON

Like processes, threads are a mechanism that permits an 
application to perform multiple tasks concurrently. A single 
process can contain multiple threads, All of these threads are
independently executing the same program, and they all share
the same global memory, including initialized, uninitialized
data, and heap segments.

The threads in a process can execute concurrently. On a 
multiprocessor system, multiple threads can execute parallel. If
one thread is blocked on I/O, other threads are still eligible
to execute.

Threads offer advantages over processes in certain applications.
For programs based on `fork()` are diffucult in sharing 
infomation between processes or relatively expensive using 
`fork()`. Threads address both of these problems:

+ Sharing information between threads is easy and fast.
+ Thread creation is faster than process creation -typically, 
  ten times faster or better.

Besides global memory, threads also share a number of other
attributes:

+ process ID and parent process ID;
+ process group ID and session ID;
+ controlling terminal;
+ process credentials (user and group IDs);
+ open file descriptors;
+ record locks created using `fcntl()`;
+ signal disposition;
+ file syste -related information: umask, current working 
  directory, and root directory;
+ interval timers (`setitimer()`) and POSIX timers 
  (`timer_create()`);
+ System V semaphore undo values;
+ resource limits;
+ CPU time consumed (as returned by `times()`);
+ resources consumed (as returned by `getrusage()`); and
+ nice value (set by `setpriority()` and `nice()`).

Amoung the attributes that are distinct for each thread are the
following:

+ thread ID;
+ signal mask;
+ thread-specific data;
+ alternate signal stack (`sigaltstack()`);
+ the `errno` variable;
+ floating-point environement;
+ realtime scheduling policy and priority;
+ CPU affinity;
+ capabilities;
+ stack.

Pthreads data types:

+ `pthread_t`:           Thread identifier
+ `pthread_mutex_t`:     Mutex
+ `pthread_mutexattr_t`: Mutex attributes object
+ `pthread_cond_t`:      Condition variable
+ `pthread_condattr_t`:  Condition variabe attributes object
+ `pthread_key_t`:       Key for thread-specific data
+ `pthread_once_t`:      One-time initialization control context
+ `pthread_attr_t`:      Thread attributes object

In the traditional UNIX API, `errno` is a global integer 
variable. Howover, this doesn't suffice for threaded programs. 
In threaded programs, each thread has its own `errno` value.

The traditional method of returning status from system calls and
some library functions is to return  on success and - on error,
with `errno` being set to indicate the error. The functions in
the Pthreads API do things differently. All Pthreads functions 
return  on success or a positive value on failure. The failure
value is one of the same values that can be placed in `errno` by
traditional UNIX system calls.


`----------------------------------------------------------------`


# THREADS: THREADS SYNCHRONIZATION

In this chapter, we describe two tools that threads can use to
synchronize their actions: mutexes and condition variables. 
Mutexes allow threads to synchronize their use of a shared 
resource, so that, for example, one thread doesn't try to access
a shared variable at the same time as another thread is modifying
it. Condition variables perform a complementary task: they allow
threads to inform each other that a shared variable has changed
state.

## Protecting Accesses to Shared Variables: Mutexes

A mutex has two states: `locked` and `unlocked`. At any moment, at
most one thread may hold the locks on a mutex. Attempting to lock
a mutex that is already locked either blocks or fails with an 
error, depending on the method used to place the lock.

When a thread locks a mutex, it becomes the owner of that mutex.
Only the mutex owner can unlock the mutex. In general, we employ
a different mutex for each shared resource (which may consists
of multiple related variables).

The static initializer value PTHREAD_MUTEX_INITIALIZER can be used
only for initializing a statically allocated mutex with default 
attributes. In all other cases, we must dynamically initialize the
mutex using `pthread_mutex_init()`.

Amoung the cases where we must use `pthread_mutex_init()` rather
than a static initializer are the following:

+ The mutex was dynamically allocated on the heap. For example, 
  suppose that we create a dynamically allocated linked list of
  structures, and each structure in the list includes a 
  `pthread_mutex_t` filed that holds a mutex that is used to 
  proctect access to that structure.
+ The mutex is an automatic variable allocated on the stack.
+ We want to initialize a statically allocated mutex with
  attributes other than the defaults.

When an automatically or dynamically allocated mutex is no longer
required, it should be destroyed using `pthread_mutex_destroy()`.
(It is not necessart to call `pthread_mutex_destroy()`) on a mutex
that was statically initialized using PTHREAD_MUTEX_INITIALIZER).

In the preceding pages, we made a number of statements about the
behaviour of mutexes:

+ A single thread may not lock the same mutex twice.
+ A thread may not unlock a mutex that it doesn't currently own.
+ A thread may not unlock a mutex that is not currently locked.

SUSv3 defines the following mutex types:

+ PTHREAD_MUTEX_NORMAK
+ PTHREAD_MUTEX_ERRORCHECK
+ PTHREAD_MUTEX_RECURSIVE
+ ...

## Signaling Changes of State: Condition Variables

A mutex prevents multiple threads from accessing a shared variable
at the same time. A condition variable allows one thread to inform
other threads about changes in the sate of shared variable (or 
other shared resource) and allows the other threads to wait 
(block) for such notification.

A condition variable is always used in conjunction with a mutex.
The mutex provides mutual exclusive for accessing the shared 
variable, while the condition variable is used to signal changes
in the variable's state.

The principal condition variable operations are `signal` and 
`wait`. The signal operation is a notification to one or more
waiting threads that a shared variable's state has changed. The 
wait opertion is the means of blocking until such a notification
is received.

The `pthread_cond_signal()` and `pthread_cond_broadcast()` 
functions both signal the condition variable specified by `cond`.
The `pthread_cond_wait()` function blocks a thread until the
condition variable `cond` is signaled.
            
Given these assumptions, `pthread_cond_signal()` can be more
efficient than `pthread_cond_broadcast()`, because it avoids the
following possibility:

+ All waiting threads are awoken.
+ One thread is scheduled first.
+ Each of remaining threads in turn locks the mutex and tests
  the state of the shared variable.

We noted earlier that a condition variable always has an associated
mutex. Both of these objects are passed as arguments to 
`pthread_cond_wait()`, which performs the following steps:

+ unlock the mutex specified by `mutex`.
+ block the calling thread until another thread signals the 
  condition variable `cond`; and
+ relock `mutex`.

There is a natural association of a mutex with a condition 
variable:

+ The thread locks the mutex in preparation for checking the state
  of the shared variable.
+ The state of the shared variable is checked.
+ If a shared variable is not in the desired state, then the 
  thread must unlock the mutex (so that other threads can access 
  the shared variable) before it goes to sleep on the condition 
  variable.
+ When the thread is reawakened because the condition variable has
  been signaled, the mutex must once more be locked, sinc, 
  typically, the thread then immediately accesses the shared 
  variable.


`-----------------------------------------------------------------`


# THREADS: THREAD SAFETY AND PER-THREAD STORAGE

This chapter extends the discussion of the POSIX threads API, 
providing a description of thread-safe functions and one-time 
initialization. We also discuss how to use thread-specific data or
thread-local storage to make an existing function thread-safe 
without changing the function's interface.

## Thread Safety (and Reentrancy Revisited)

A function is said to be `thread-safe` if it can safely be invoked
by multiple threads at the same time. A `reentrant` function achives
thread safety without the use of mutexes.

## One-Time Initialization

Sometimes, a threaded application needs to ensure that some
initialization action occurs just once, regardless of how many 
threads are created. A library function can perform one-time
initialization using the `pthread_once()` function.

## Thread-Specific Data

The most efficient way of making a function thread-safe is to make
it reentrant. Thread-specific data is a technique for making an
existing function thread-safe without changing its interface. Thread-
specific data allows a function to maintain a separate copy of a
variable for each thread that calls the function.

## Thread-Local Storage

Like thread-specific data, thread-local storage provides persistent
per-thread storage. The main advantage of thread-local storage is 
that it is much simpler to use than thread-specific data. To create
a thread-local variable, we simply include the `__thread` specifier
in the declaration of a global or static variable.


`------------------------------------------------------------------`


# THREADS: THREAD CANCELLATION

Sometimes, it can be useful to `cancel` a thread; that is, to send
it a request asking it to terminate now. 

## Cleanup Handlers

If a thread pending calcellation were simply terminated when it
reached a cancellation point, then shared variables and Pthreads
objects (e.g., mutexes) might be left in an inconsistent states. 
To get around this problem, a thread can establish one or more
`cleanup handler`. A cleanup handler can perform tasks such as 
modifying the values of global variables and unlocking mutexes
before the thread is terminated.