#include "sys_socket.h"
#include "sys_pthread.h"
#include "sys_time.h"
#include "ANO_FUNC.h"
#include <sys/socket.h>
#include <netinet/in.h>
#include <netdb.h>
#include <arpa/inet.h>
#define UDP_PORT 7788
#define DES_PORT 1234
#define DES_IPADDR "192.168.1.11"
#define ANO_THREADNAME "ANO"

INT32 ANO_udp_init()
{
    INT32 sockfd;
    sockfd = sys_socket_create(AF_INET,SOCK_DGRAM,0);
    if(ERROR == sockfd)
    {
        return ERROR;
    }
    sys_socket_bind(sockfd,AF_INET,NULL,UDP_PORT);
    return sockfd;
}

int ANO_udp_send(int sockfd,void *pBuf, int iLen)
{
    return sys_socket_send_to(sockfd,pBuf,iLen,AF_INET,DES_IPADDR,DES_PORT);
}

int ANO_udp_recv(int sockfd,void *pBuf, int iBufLen)
{
    CHAR recvip[512];
    UINT16 port = 0;
    INT32 len = 0;
    len =  sys_socket_recvfrom(sockfd, pBuf,iBufLen,recvip, (UINT16)strlen(DES_IPADDR), &port);
    return len;
}


VOID ANO_recvwork_thread(INT32 ANO_sockfd)
{
  INT32 len = 0;
  while(TRUE)
  {
        len = ANTO_Send(ANTO_SENSOR,ANO_sockfd);
        sys_time_sleep_ms(10);
  }
}


int ANO_udp_Init()
{
    INT32 ANO_sockfd = -1;
    INT32 iRet = -1;
    TASK_ID   ANO_ThreadID; 
    ANO_sockfd = ANO_udp_init();
    iRet = sys_pthread_create(&ANO_ThreadID, ANO_THREADNAME, TASK_PRIORITY_5, SIZE_32KB, (FUNCPTR)ANO_recvwork_thread,1,ANO_sockfd);
    if(iRet != OK)
    {
        printf("create ANO task failed!\n");
        return -1;
    }
    return 0;

}

int ANO_Init()
{
    return ANO_udp_Init();
}