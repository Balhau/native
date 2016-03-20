#define _GNU_SOURCE                 //This is to be able t use the RTLD_NEXT constant
#include <dlfcn.h>                  //This is to use the dlsym symbol
#include <stdio.h>                  //This is for printf calls
#include <string.h>
#include <sys/types.h>              //This is for sendto call
#include <sys/socket.h>             //This is for sendto call

//Pointers for libc calls

//Revert the string
char * strrev(char *str)

{
    int i = strlen(str)-2,j=0;

    char ch;
    while(i>j)
    {
        ch = str[i];
        str[i]= str[j];
        str[j] = ch;
        i--;
        j++;
    }
    return str;
}

/*
Function prototypes
*/
static  ssize_t (*real_sendto)
              (int fd, const __ptr_t buf, size_t n, int flags,
              	  __CONST_SOCKADDR_ARG addr, socklen_t addr_len) = NULL;

static ssize_t (*real_write)(int fd, const void *buf, size_t count) = NULL;

static ssize_t (*real_send)(int sockfd, const void *buf, size_t len, int flags) = NULL;

ssize_t sendto(int fd, const __ptr_t buf, size_t n, int flags,
	  __CONST_SOCKADDR_ARG addr, socklen_t addr_len){

      fprintf(stdout,"sendto_call: %s\n",buf);
      fflush(stdout);

      real_sendto = dlsym(RTLD_NEXT, "sendto");
      return real_sendto(fd,buf,n,flags,addr,addr_len);
}

ssize_t write(int fd,const void *buf,size_t count){
  fprintf(stdout, "write_call: %s\n",buf);
  fflush(stdout);

  real_write=dlsym(RTLD_NEXT,"write");
  return real_write(fd,buf,count);
}

ssize_t send(int fd,const void *buf,size_t len,int flags){
  fprintf(stdout,"Send call: %s\n",buf);
  fflush(stdout);

  real_send=dlsym(RTLD_NEXT,"send");
  return real_send(fd,buf,len,flags);
}
