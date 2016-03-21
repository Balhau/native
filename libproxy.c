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

void printOut(const char* label,const char* buf){
  fprintf(stderr,"%s: %s\n",label,buf);
  fflush(stderr);
}

/*
Function prototypes
*/
static  ssize_t (*real_sendto)
              (int fd, const __ptr_t buf, size_t n, int flags,
              	  __CONST_SOCKADDR_ARG addr, socklen_t addr_len) = NULL;

static ssize_t (*real_write)(int fd, const void *buf, size_t count) = NULL;

static ssize_t (*real_send)(int sockfd, const void *buf, size_t len, int flags) = NULL;

static int (*real_open)(const char *pathname, int flags)=NULL;

static ssize_t (*real_read)(int fd, void *buf, size_t count)=NULL;

static ssize_t (*real_recv)(int s, void *buf, size_t len, int flags)=NULL;


ssize_t sendto(int fd, const __ptr_t buf, size_t n, int flags,
	  __CONST_SOCKADDR_ARG addr, socklen_t addr_len){

      printOut("sendto_call",buf);

      real_sendto = dlsym(RTLD_NEXT, "sendto");
      return real_sendto(fd,buf,n,flags,addr,addr_len);
}

ssize_t write(int fd,const void *buf,size_t count){

  printOut("write_call",buf);

  real_write=dlsym(RTLD_NEXT,"write");
  return real_write(fd,buf,count);
}

ssize_t send(int fd,const void *buf,size_t len,int flags){
  printOut("send_call",buf);
  real_send=dlsym(RTLD_NEXT,"send");
  return real_send(fd,buf,len,flags);
}

ssize_t read(int s,void *buf,size_t len){
  real_read=dlsym(RTLD_NEXT,"read");
  ssize_t retVal=real_read(s,buf,len);

  printOut("read_call",buf);

  return retVal;
}

ssize_t recv(int s,void *buf,size_t len,int flags){
  real_recv=dlsym(RTLD_NEXT,"recv");
  ssize_t retval=real_recv(s,buf,len,flags);
  printOut("recv_call",buf);

  return retval;
}


int open(const char *path,int flags){
    printOut("open_call",path);

  real_open=dlsym(RTLD_NEXT,"send");
  return real_open(path,flags);
}
