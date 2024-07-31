# SIGNALS: FUNDAMENTAL CONCEPTS

## Concepts an Overview

A `signal` is a notification to a process that an event has occured. 
Signals are sometimes described as `software interrupts`. One process can
(if it has suitable permissions) send a signal to another process. Howover
, the usual source of many signals sent to a process is the kernel. Amoung
the types of events that cause the kernel to generate a signal for a 
process are the following:

+ A hardware exception occured.
+ The user typed one of the terminal special characters that generate 
  signals.
+ A software event occured.

Signals falls into two broad categories. The first set constitutes the 
`traditional` or `standard` signals, which are used by the kernel to 
notify processes of events. On linux, the standard signals are numbered 
from 1 to 31. The another set of signals consists of the `realtime`
signals.

Sometimes, we nned to ensure that a segment of code is not interrupted
by the delivery of a signal. To do this, we can add a signal to the 
process's `signal mask` - a set of signals whose delivery is currently
`blocked`.

Upon delivery of a signal, a process carries out one of the following 
default actions:

+ The signal is `ingored`.
+ The process is `terminated` (kiled).
+ A `core dump file` is generated. 
+ The process is `stopped`.
+ Execution of the process is `resumed`.

Instead of accepting the default for a particular signal, a program can
change the action (`disposition`):

+ The `default action` should occur.
+ The signal is `ignored`.
+ A `signal handler` is executed.

## Signal Types and Default Actions

The following list describes the various signals:

+ SIGABRT
+ SIGALRM
+ SIGBUS
+ SIGCHLD
+ SIGCONT
+ SIGEMT
+ SIGFPE
+ SIGHUB
+ SIGILL
+ SIGINFO
+ SIGINT
+ SIGIO
+ SIGKILL
+ SIGLOST
+ SIGPIPE
+ SIGPROF
+ SIGPWR
+ SIGQUIT
+ SIGSEGV
+ SIGSTKFLT
+ SIGSTOP
+ SIGSYS
+ SIGTERM
+ SIGTRAP
+ SIGTSTP
+ SIGTIN
+ SIGTOU
+ SIGUNUSED
+ SIGURG
+ SIGUSR1
+ SIGUSR2
+ SIGVTALRM
+ SIGWINCH
+ SIGXCPU
+ SIGXFSZ

## Sending Signals: `kill()`

One process can send a signal to another process using the `kill()` 
system call.

## Signal Sets

Many signal-related system calls need to be able to represent a group of
different signals.

## The Signal Mak (Blocking Signal Delivery)

For each process, the kernel maintains a `signal mask` - a set of signals
whose delivery to the process is currently blocked.If a signal that is
blocked is sent to process, delivery of that signal is delayed until it is
unblocked by being removed from the process signal mask.

## Pending Signals

If a process recieves a signal that it is currently blocking, that signal
is added to the process's set of pending signals. When (and if) the signal
is later unblocked, it is then delivered to the process. To determine
which signals are pending for a process, we can call `sigpending()`.

## Signal Are Not Queued

The set of pending signals is only a mask; it indicates whether or not a
signal has occured, but not how many times it occurred. (One of the 
differences between standard and realtime signals is that realtime signals
are queued.)

## Changing Signal Dispositions: `sigaction()`

The `sigaction` system call is an alternative to `signal()` for setting
the disposition of a signal. 


`-----------------------------------------------------------------------`


# SIGNALS: SIGNAL HANDLERS

## Designing Signal Handlers

Two common designs for signal handlers are the following:

+ The signal handler sets a global flag and exits. The main program 
  periodically checks this flag and, if it is set, takes appropriata 
  action.
+ The signal handler performs some type of cleanup an then either 
  terminate the process or uses a nonlocal goto to unwind the stack and
  return control to a predetermined location in the main program.

### Reentrant and Async-Signal-Safe Functions

Not all system calls and library functions can be safely called from a 
signal handler. To understand why requires an explanation of two concepts:
reentrant and async-signal-safe functions.

A function is said to be `reentrant` if it can safely be simultaneously
executed by multiple threads of execution in the same process. In this
context, "safe" means that the function achives its expected result, 
regardless of the state of execcution of any other thread of function.

A function may be `nonreentrant` if it updates global or static data 
structures. (A function that employs only local variables is guaranted
to be reentrant.) 

### Standard async-signal-safe functions

An `async-signal-safe` function is one that the implementation guarantees to be
safe when called from a signal handler. A function is async-signal-safe either
because it is reentrant or because it is not interruptable by a signal handler.

### Handling a Signal on an Alternate Stack: `sigaltstack()`:

Normally, when a signal handler is invoked, the kernel creates a frame for it 
on the process stack. Howover, this may not be poosible if a process attempts 
to extend the stack beyond the maximum possible size. When a process attempts
to grow its stack beyond the maximum possible size, the kernel generates a
SIGSEGV signal for the process.

If we instead need to ensure that the SIGSEGV signal is handled in these
circumstances, we can do the following:

+ Allocate an area of memory, called an `alternate signal stack`, to be used
  for the stack frame of a handler.
+ Use the `sigaltstack()` system call to inform the kernel of the existance
  of the alternate signal stack.
+ When establishing the signal handler, specify the SA_ONSTACK flag, to tell
  the kernel that the frame for this handler should be created on the alternate
  stack.


`----------------------------------------------------------------------------`


# SIGNALS: ADVANCED FEATURES

## Core Dump Files

Certain signals cause a process to create a core dump and terminate. A core
dump is a file containing a memory image of the process at the time it 
terminated.

## Special Cases for Delivery, Dispositon and Handling

### SIGKILL and SIGSTOP

It is not possible to change the default action for SIGKILL, which always 
terminates a process, and SIGSTOP, which always stops a process. Both 
`signal()` and `sigaction` return an error on attempts to change the 
disposition of these signals. These two signals also can't be blocked.

### SIGCONT and stop signals

The SIGCONT signal is used to continue a process previously stopped by one of
the stop signals (SIGSTOP, SIGTSTP(), and SIGTTOU). If a process is currently
stopped, the arrival of a SIGCONT signal always causes the process to resume,
even if the process is currently blocking SIGCONT. Whenever SIGCONT is 
delivered to a process, any pending stop signals for the process are discarded
(i.e., the process never sees them).

## Hardware-Generated Signals

SIGBUS, SIGFPE, SIGILL, and SIGSEGV can be generated as a consequence of a 
hardware exception or, less usually, by being sent by `kill()`.

## Realtime Signals

Realtime signals have the following advantages over standard signals:

+ Realtime signals provide an increment range of signals that can be used for
  application-defined purposes. Only two standard signals are freely used for 
  application-defined purposes: `SIGUSR1` and `SIGUSR2`.
+ Realtime signals are queued.
+ When sending a realtime signal, it is possible to specify data (an integer
  or pointer value) that accompanies the signal. The signal handler is the
  receiving process can retrieve this data.
+ The order of delivery of different realtime signals is guaranteed.

Realtime signals are not individually identified by different constants in the
manner of standard signals. A realtime signal number can be reffered to by 
adding a value to `SIGRTMIN`; for example, the expresseion (`SIGRTMIN` + 1)
refers to the second realtime signal.

In order for a pair of processes to send and receive realtime signals, SUSv3
requires the following:

+ The sending process sends the signal plus its accompanying data using the
  `sigqueue` system call.
+ The receiving process establishes a handler for the signal using a call to 
  `sigaction()` that specifies the SA_SIGINFO flag.

## Waiting for a Signal Using a Mask: `sigsuspend()`

Suppose that the SIGINT signal is delivered after execution of the second
`sigprocmask()`, but before the `pause()` call. Delivery of the SIGINT signal
will cause the handler to be invoked, and after the handler returns and main
program resumes, the `pause()` can will block until a `second` instance of 
SIGINT is delivered. This defeats the purpose of the code, which was to unblock
SIGINT and then wait for its `first` occurrence.

To avoid this problem, we require a means of `automically` unblocking a signal
and suspend the process. 

