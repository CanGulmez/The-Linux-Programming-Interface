# Memory Allocation

Many system programs need to be able to allocate extra memory for 
dynamic data structures (e.g., linked lists and binary trees), whose
size depends on information that is avaliable only at run time.

## Allocating Memory on the Heap

The current limit of the heap is reffered to as the `program break`.
Resizing the heap is actually as simple as telling the kernel to
adjust its idea of where the process's program break is. Initially,
the program break lies just past the end of uninitialized data 
segment.

### Adjusting the Program Break: `brk()` and `sbrk()`

The `brk()` system call sets the program break to the location
specified by its parameter. A call to `sbrk()` adjust the program
break by adding `increment` to it. The call `sbrk(0)` returns the
current setting of the program break without changing it.

### Allocating Memory on the Heap: `malloc()` and `free`

In general, C programs use the `malloc` family of functions to 
allocate and deallocate memory on the heap. These functions offer
several advantages:

+ are standardized as part of the C language;
+ are easier to use in threaded programs;
+ provide a simple interface that allows memory to be allocated in
  small units; and
+ allow us to arbitrary deallocate blocks of memory, which are 
  maintained on a free list and recycled in future calls to allocate
  memory.

## Allocating Memory on the Stack: `alloca()`

`alloca()` allocates memory dynamically. Howover, instead of 
obtaining memory from the heap, `alloca()` obtains memory from the
stack by increasing the size of the stack frame.