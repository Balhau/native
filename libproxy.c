#define _GNU_SOURCE                 //This is to be able t use the RTLD_NEXT constant
#include <dlfcn.h>                  //This is to use the dlsym symbol
#include <stdio.h>                  //This is for printf calls
#include <string.h>
#include <sys/types.h>              //This is for sendto call
#include <sys/socket.h>             //This is for sendto call

//Pointers for libc calls

static  ssize_t (*real_sendto)
              (int sockfd, const void *buf, size_t len, int flags,
              const struct sockaddr *dest_addr, socklen_t addrlen
            ) = NULL;

static ssize_t (*real_write)(int fd, const void *buf, size_t count) = NULL;

ssize_t sendto(int sockfd,
                const void *buf, size_t len, int flags,
                const struct sockaddr *dest_addr, socklen_t addrlen){

      fprintf(stdout,"sendto_call: %s",buf);
      fflush(stdout);

      real_sendto = dlsym(RTLD_NEXT, "sendto");
      return real_sendto(sockfd,buf,len,flags,dest_addr,addrlen);
}

ssize_t write(int fd,const void *buf,size_t count){
  fprintf(stdout, "write_call: %s",buf);
  fflush(stdout);
  real_write=dlsym(RTLD_NEXT,"write");
  return real_write(fd,buf,count);
}
