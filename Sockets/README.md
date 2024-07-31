# SOCKETS: INTRODUCTION

## Overview

In typical client-server scenario, applications communicate using 
sockets as follows:

+ Each application creates a socket. A socket is the "apparatus" 
  that allows communication, and both applications require one.
+ The server binds its socket to a well-known address (name) so that
  clients can locate it.

### Communication domains

Sockets exist in a `communication domain`, which determines:

+ the method of identifying a socket (i.e., the format of a socket 
  "address"); and
+ the range of communication (i.e., either between applications on 
  the same host or between applications on different hosts connected
  via a network).

Modern operating systems support at least the following domains:

+ The UNIX (`AF_UNIX`) domain allows communication between 
  applications on the same host.
+ The `IPv4` (`AF_INET`) domain allows communication between 
  applications running on hosts connected via an Internet Protocol
  version 4 (IPv4) network.
+ The `IPv6` (`AF_INET6`) domain allows communication between 
  applications running on hosts connected via an Internet Protocol
  version 6 (IPv6) network.

### Socket types

Every sockets implementation provides at least two types of sockets:
streams and datagram.

`Stream sockets` (`SOCK_STREAM`) provide a reliable, biderectional,
byte-stream communication channel.

`Datagram sockets` (`SOCK_DGRAM`) allow data to be exchanged in the
form of messages called `datagrams`.

In the Internet domain, datagram sockets employ the User Datagram
Protocol (UDP), and stream sockets (usually) employ the Transmission
Control Protocol (TCP).

### Socket system calls

The key socket system calls are the following:

+ The `socket()` system call creates a new socket.
+ The `bind()` system call binds a socket to an address.
+ The `listen()` system call allows a stream socket to accept 
  incoming connections from other sockets.
+ The `accept()` system call accepts a connection from a peer
  application on a listening stream socket.
+ The `connect()` system call establishes a connection with 
  another socket.

## Stream Sockets

The operation of stream sockets can be explained as follows:

+ The `socket()` system call, which creates a socket. In order for
  two applications to communicate, each of them must create a 
  socket.
+ One application must connect its socket to another application's
  socket before communication can take place. Two sockets are 
  connected as follows:
  + One application calls `bind()` in order to bind the socket to a
    well-known address, and then calls `listen()` to notify the 
    kernel of its willingness to accept incoming connections.
  + The other application establishes the connection by calling
    `connect()`, specifying the address of the socket to which the
    connection is to be made.
  + The application that called `listen()` then accepts the 
    connection using `accept()`.
+ Once a connection has been established, data can be transmitted in
  both directions between applications.

### Active and passive sockets

Stream sockets are often distinguished as being either active or
passive:

+ By default, a socket that has been created using `socket()` is 
  `active`. An active socket can be used in a `connect()` call to
  establish a connection to passive socket.
+ A `passive` socket  (also called a `listening` socket) is one 
  that has been marked to allow incoming connections by calling
  `listen()`.

In most applications that employ stream sockets, the server performs
the passive openi and the client performs the active open.

Overview of system calls used with stream sockets:

   `Passive socket`                
      (server)
         |
      socket()
         |
       bind()
         |
      listen()
         |
      accept()                   `Active Socket`
         |                          (client)
         |  (blocks until              |
         | client connects)         socket()
         |                             |
         | <----------------------- connect()
         |                             |
       read() <--------------------- write()
         |                             |
      write() ---------------------> read()
         |                             |
      close()                       close()

## Datagram Sockets

The operation of datagram sockets can be explained as follows:

+ Each application that wants to send or receive datagram creates a
  datagram socket using `socket()`.
+ In order to allows another application to send it datagrams (
  letters), an application uses `bind()` to bind its socket to a 
  well-known address.
+ To send a datagram, an application calls `sendto()`, which takes
  as one of its arguments the address of the socket to which
  datagram is to be sent.
+ In order to receive a datagram, an application calls `recvfrom()`
  which may block if no datagram has yet arrived.
+ When the socket is no longer needed, the application closes it
  using `close()`.

Overview of system calls used with datagram sockets:
               
   `Server`
      |
   socket()
      |
    bind()                
      |                          `Client`
      |  (blocks until              |
      | client connects)         socket()
      |                             |
      |                             |
   recvfrom() <------------------ sendto()
      |                             |
   sendto() -------------------> recvfrom()
      |                             |
    close()                       close()

### Using `connect()` with Datagram Sockets

Even though datagram sockets are connectionless, the `connect()`
system call serves a purpose when applied to datagram sockets. 
Calling `connect()` on a datagram socket causes the kernel to record
a particular address as this socket's peer. The term `connected`
`datagram socket` is applied to such a socket. The term 
`unconnected datagram socket` is applied to a datagram socket on 
which `connect()` has not been called.

After a datagram socket has been connected:

+ Datagrams can be sent through the socket using `write()` (or
  `send()`) and are automatically sent to the same peer socket.
+ Only datagrams sent by the peer socket may be read on the socket.

The abvious advantage of setting the peer for a datagram socket is
that we can use simpler I/O system calls when transmitting data on
the socket. 


`-------------------------------------------------------------------`


# SOCKETS: UNIX DOMAIN

This chapter looks at the use of UNIX domain sockets, which allow
communication between processes on the same host system. 

The following points are worth noting about binding a UNIX domain
socket:

+ We can't bind a socket to an existing pathname.
+ It is usual to bind a socket to an absolute pathname.
+ A socket may be bound to only one pathname; conversely, a pathname
  can be bound to only one socket.
+ We can't use `open()` to open a socket.
+ When the socket is no longer required, its pathname entry can (and
  generally should) be removed using `unlink()` (or `remove()`).


`-------------------------------------------------------------------`


# SOCKETS: FUNDAMENTALS OF TCP/IP NETWORKS

This chapter provides an introduction to computer networking 
concepts and the TCP/IP networking protocols.

## Internets

An `internetwork` or commonly `internet`, connects different computer
networks, allowing hosts on all of the networks to communicate with
one another.

An internet aims to hide the details of different pyhsical networks
in order to present a unified network architecture to all hosts on
the connected networks. 

Although various internetworking protocols have been devised, TCP/
IP has become the dominant protocol suite, supplanting even the
proprietary networking protocols that were formerly common on local
and wide area networks. The term `Internet` is used to refer to the
TCP/IP internet that connects millions of computers globally.

An internet using a router to connect two networks:

   `pukaki`             `rotoiti`
      |                     |
      |      Network 1      |
   ------------------------------
               |
            `tekapo` Router
               |
   ------------------------------
      |     Network 2       |
      |                     |
   `wakatipu`           `wanaka`

## Networking Protocols and Layers

A `networking protocol` is a set of rules defining how information
is to be transmitted accross a network. The `TCP/IP protocol suite`
is a layered networking protocol.

Protocols in the TCP/IP suite:

   `Application`        `Application`        `Application`
        |                     |                    |
        | SOCK_DGRAM          | SOCK_RAW           | SOCK_STREAM
        |                     |                    |
      `UDP` ----------------> | <--------------- `TCP` (Transport
                              |                           layer)
                             `IP` (Network layer)
                              |
                              |
      (Hardware) `Network interface hardware` (Data-link layer)
                  
## The Data-Link Layer

The lowest layer is the `data-link` layer, which consists of the
device driver and the hardware interface (network card) to the 
underlying physical medium. The data-link layer is concerned with 
transferring data accross a physical link in a network.

To transfer data, the data-link layer encapsulates datagrams from 
the network layer into units called `frames`. In addition to the 
data to be transmitted, each frame includes a header containing, for
example, the destination address and frame size. The layer may 
perform error detection, retransmission, and flow control. Some data
-link layers also split large network packets into multiple frames
and reassemble them at the receiver.

One characterictic of the data-link layer that is important for our
discussion of IP is the `maximum transmission unit` (MTU). A data-
link layer's MTU is the upper limit that the layer places on the 
size of a frame.

## The Network Layer: IP

Above the data-link layer is the `network layer`, which is concerned
with delivering packets (data) from the source host to destination
host. This layer performs a variety of tasks, including:

+ breaking data into fragment small enough for transmission via the
  data-link layer;
+ routing data accross the internet; and
+ providing services to the transport layer.

### IP transmits datagrams

IP transmits data in the form of datagrams (packets). Each datagram
sent between two hosts travels independently accross the network, 
possibly taking a different route. An IP datagram includes a header,
which ranges in size from 20 to 60 bytes. The header contains the
address of the target host, so that the datagram can be routed 
through the network to its destination, and also includes the 
orignating address of the packet, so that the receiving host knows
the origin of the datagram.

### IP is connectionless and unreliable

IP is described as a `connectionless` protocol, since it doesn’t 
provide the notion of a virtual circuit connecting two hosts. IP is 
also an `unreliable` protocol: it makes a “best effort” to transmit 
datagrams from the sender to the receiver, but doesn’t guarantee 
that packets will arrive in the order they were transmitted, that 
they won’t be duplicated, or even that they will arrive at all.

### IP may fragment datagrams

IPv4 datagrams can be up to 65,535 bytes. By default, IPv6 allows 
datagrams of up to 65,575 bytes (40 bytes for the header, 65,535 
bytes for data), and provides an option for larger datagrams 
(so-called `jumbograms`).

## IP Addresses

An IP address consists of two parts: a network ID, which specifies
the network on which a host resides, and a host ID, which identifies
the host within that network.

### IPv4 addresses

An IPv4 address consists of 32 bits:

                 ------------------------
Network address: | Network ID | Host ID |
                 ------------------------
Network mask:    | all 1s     | all 0s  |
                 ------------------------

Certain IPv4 addresses have special meanings. The special address
`127.0.0.1` is normally defined as the `loopback address`, and is
conventionally assigned the hostname `localhost`. A datagram sent
to this address never actually reaches the network, but instead 
automatically loops back to become input to the sending host.

The constant `INADDR_ANY` is the so-called IPv4 `wildcard addresses`.
The wildcard IP address is useful for application that bind Internet
domain sockets on multihomed hosts.

### IPv6 addresses

The key difference is that IPv6 addresses consist of 128 bits, and
the first few bits of the address are a `format prefix`, indicating
the address type.

IPv6 addresses are typically written as a series of 16-bit:

   `F000:0:0:0:0:0:A:1`

IPv6 addresses often include a sequence of zeros and, as a notational
convenience, two colons (::) can be employed as follows:

   `F000::A:1`

Format of an IPv4-mapped IPv6 address

---------------------------------------------------
|      all zeros           |  FFF  | IPv4 address | 
---------------------------------------------------
        80 bits            16 bits     32 bits

## The Transport Layer

There are two widely used transport-layer protocols in the TCP/IP
suite:

+ `User Datagram Protocol` (UDP)
+ `Transmission Control Protocol` (TCP)

### Port Numbers

The task of the transport protocol is to provide an end-to-end 
communication services to applications residing on different hosts.
In TCP and UDP, this differentiation is provided by a 16-bit `port`
number.

Some `well-known port numbers` are permanently assigned to specific 
application.

If an application doesn't select a particular port, then TCP and UDP
assign a unique `ephemeral port`.


`-------------------------------------------------------------------`


# SOCKETS: INTERNET DOMAINS

We are now ready in this chapter to look at programming with sockets
in the IPv4 (`AF_INET`) and IPv6 (`AF_INET6`) domains.

## Internet Domain Sockets

Internet domain stream sckets are implemented on top of TCP.

## Network Byte Order

IP addresses and port numbers are integer values. One problem we
encounter when passing these values accross a network is that 
different hardware architectures store the bytes of a multibyte 
integer in different orders.

Since port numebrs and IP addresses must be transmitted between, and
understood by, all hosts on a network, a standard ordering must be
used. This ordering is called `network byte order`, and happens to
be big endian.

## Overview of Host and Service Conversion Functions

Computers represent IP addresses and port numbers in binary. Howover,
humans find names easier to remember than numbers. Employing symbolic
names also provides a useful level of indirection; users and programs
can continue to use the same name even if the underlying numeric
value changes.

A `hostname` is the symbolic identifier for a system that is 
connected to a network (possibly with multiple IP addresses). A 
`service name` is the symbolic representation of a port name.

The following methods are avaliable for representing host addresses
and ports:

+ A host addresses can be represented as a binary value, as a 
  symbolic hostname, or in presentation format (dotted-decimal for
  IPv4 or hex-string for IPv6).
+ A port can be representated as a binary value or as a symbolic
  service name.

## Domain Name System (DNS)

DNS is used to maintain the mappings between hostnames and IP 
addresses. Before the advent of DNS , mappings between hostnames 
and IP addresses were defined in a manually maintained local file, 
`/etc/hosts`, containing records.

Howover, the `/etc/hosts` scheme scales poorly, and then becomes 
impossible, as the number of hosts in the network increases (e.g., 
the Internet, with millions of hosts).

DNS was devised to address this problem. The key ideas of DNS are the
following:

## The `/etc/services` File

Well-known port numbers are centrally registered by IANA. Each of
these ports has a corresponding `service name`. The `getaddrinfo()`
and `getnameinfo()` functions use the information in this file to
convert services names to port numbers and vice versa.

## Protocol-Independent Host and Service Conversion

The `getaddrinfo()` function converts host and service names to IP
addresses and port numbers. The `getnameinfo()` function is the 
converse of `getaddrinfo()`. It translates a socket address structure
(either IPv4 or IPv6) to strings containing the corresponding host
and service name.