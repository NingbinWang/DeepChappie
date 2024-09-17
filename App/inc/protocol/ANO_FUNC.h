#ifndef _ANO_FUNC_
#define _ANO_FUNC_
#include "ANO_DT.h"

int ANO_Init();
int ANO_udp_send(int sockfd,void *pBuf, int iLen);
int ANO_udp_recv(int sockfd,void *pBuf, int iBufLen);
int ANTO_Send(const ANTO_MODE_E mode,int sockfd);
#endif