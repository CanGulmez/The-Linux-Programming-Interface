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
  multiplexing
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

### Employing Nonblocking I/O with Alternative I/O Models

Nonblocking I/O (the `O_NONBLOCK` flag) is often used in conjunction
with the I/O models described in this chapter.

## I/O Multiplexing

I/O multiplexing allows us to simultaneously monitor multiple file
descriptors to see if I/O is possible on any of them.