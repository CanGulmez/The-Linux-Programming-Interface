# ALTERNATIVE I/O MODELS

## Overview

This chapter discusses three alternatives to the conventional file   
I/O model:

+ I/O multiplexing (the `select()` and `poll()` system calls);       
+ signal-driven I/O; and
+ the Linux-specific `epoll` API.

Because of the limitations of both nonblocking I/O and the use of 
multiple threads or processes, one of the following alternatives is
ofter preferable:

+ `I/O multiplexing` allows a process to simultaneously monitor 
  multiple file descriptors to find out whether I/O is possible on
  any of them. The `select()` and `poll()` system calls perform I/O
  multiplexing.
+ `Signal-driven I/O` is a technique whereby a process requests that
  the kernel send it a signal when input is avaliable or data can be
  written on a specified file desciptor. Signal-driven I/O provides
  significantly better performance than `select()` and `poll()`
+ The `epoll` API is a Linux-specific feature and provides better
  performance when monitoring large numbers of file descriptors.

Note that none of these techiques performs I/O. There merely tell us
that a file descriptor is ready. Some other system call must then 
be used to actually perform the I/O.

### Level-Triggered and Edge-Triggered Notification

+ `Level-triggered notification`: A file descriptor is considered to
  be ready if it is possible to perform an I/O system call without
  blocking.
+ `Edge-triggered notification`: Notification is provided if there
  is I/O activity (e.g., new input) on a file descriptor since it
  was last monitored.

The `epoll` API differs from the other two I/O models in that it can
employ both level-triggered notification (default) and edge-triggered
notification.

When we employ level-triggered notification, because the level-
triggered model allows us to repeat the I/O monitoring operation at
any time, it is not necassary to perform as much I/O as possible 
(e.g., read as many bytes as possible) on the file descriptor each 
time we are notified that a file descriptor is ready.

When we employ edge-triggered notification, we receive  notification
only when an I/O event occurs. Furthermore, when an I/O event is 
notified for a file descriptor, we usually don't know how much I/O
is possible (e.g., how many bytes are avaliable for reading).

### Employing Nonblocking I/O with Alternative I/O Models

Nonblocking I/O (the `O_NONBLOCK` flag) is often used in conjunction
with the I/O models described in this chapter.

## I/O Multiplexing

I/O multiplexing allows us to simultaneously monitor multiple file
descriptors to see if I/O is possible on any of them. We can use  
`select()` and `poll()` to monitor file descriptors for regular
files, terminals, pseudoterminals, pipes, FIFOs, sockets, and some
types of character devices. Both system calls allow a process either
to block indefinitely waiting for file descriptors to become ready
or to specify a timeout on the call.

## Signal-Driven I/O

## The `epoll` API

Like the I/O multiplexing system calls and signal-driven I/O, the 
Linux `epoll` (event poll) API is used to monitor multiple file 
descriptors to see if they are ready for I/O. The primary advantages
of the `epoll` API are the following:

+ The performance of `epoll` scales much better that `select()` and
  `poll()` when monitoring large numbers of file descriptors.
+ The `epoll` API permits either level-triggered or edge-triggered
  notification.

The performance of `epoll` and signal-driven I/O is similar. Howover,
`epoll` has some advantages over signal-driven I/O:

+ We avoid the complexities of signal handling.
+ We have greater flexibily in specifying what kind of monitoring
  we want to perform.

