# Native system calls proxy

## Intro

This is an experimental repository with some native code (target for linux) to proxy some libc calls and by doing this being able to monitor what native aplications and any language based on an interpreter is trying to do at lower level.


## Use case

Here we create a simple python application application called **send_socket_data.py** which basically opens a connection, sends some data and then closes the connection. The code is as simple as:

    import socket

    ip="127.0.0.1"
    port = 10000

    s = socket.socket()
    s.connect((ip,port))
    s.send("Hello there\r")
    s.close()

Since this assumes port 10000 open on 127.0.0.1 we need to enforce this assumption by opening it, we can do this with

    nc -vlk 10000

The next step consists in running the program and finding the libc call responsible to send the data into the network. We need to do this in two steps, first we need to list all libc symbols and then we need to trace the running script to find which is the one being called. So first we output all libc symbols into a file

    nm -D /lib/x86_64-linux-gnu/libc.so.6 > libc_symbs.txt

and then run the script with

    strace python send_socket_data.py 2> send_socket_data.strace

With this we run the python script and redirect all the system calls into a file

The following step consists in finding where the token *Hello there"* is on the strace output, we find that

    sendto(3, "Hello there\r", 12, 0, NULL, 0)

So the next step is to find if we got some symbol *sendto* in the libc. We [found](http://linux.die.net/man/2/sendto) that the signature is

    ssize_t sendto(
      int sockfd, const void * buf,
      size_t len, int flags,
      const struct sockaddr * dest_addr,
      socklen_t addrlen
    );

and

    ssize_t send(
        int sockfd,
        const void * buf,
        size_t len,
        int flags
    );

So the next step consists in create a shared object with proxy for this call and fetch it, with LD_PRELOAD, into the running script.
