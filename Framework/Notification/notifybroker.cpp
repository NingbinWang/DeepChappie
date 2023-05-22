#include "notifybroker_priv.h"


/**
  * @brief  创建一个链表，并在链表中初始化pingpong
  * @param  
  * @param  id:  Account ID
  * @retval If the search is successful, return the pointer of the account
  */
Broker_Node * notifybroker_creatnode(DataCentor_Header *head,const char* id)
{
    Broker_Node *node=NULL;
    node = (Broker_Node *)sys_mem_malloc(sizeof(Broker_Node));
    if(node == NULL){
        LOGGER_ERROR("malloc errror \n");
        return NULL;
    }
    node->next = NULL;
    strncpy(node->ID,id,strlen(id));
    if(head->BufferSize != 0)
    {
        UINT8* buf0 = NULL;
        UINT8* buf1 = NULL;
        UINT8 *buffer = (UINT8*) sys_mem_malloc((head->BufferSize) * sizeof(UINT8) * 2);
        if (!buffer)
        {
            LOGGER_ERROR("buffer malloc failed!");
            return ERROR;
        }
        memset(buffer, 0, (head->BufferSize) * sizeof(UINT8) * 2);
        buf0 = buffer;
        buf1 = buffer + (head->BufferSize);
        PingPongBuffer_Init(&node->BufferManager, buf0, buf1);
    }else{
       LOGGER_ERROR("buffer malloc size is %d!",head->BufferSize);
    }
    return node;
}


DataCentor_Header * notifybroker_initdatacentor(const char* name,UINT32 pingpongbuffsize)
{
    DataCentor_Header *node=NULL;
    node = (DataCentor_Header *)sys_mem_malloc(sizeof(DataCentor_Header));
    if(node == NULL){
        LOGGER_ERROR("malloc errror \n");
        return NULL;
    }
    node->next = NULL;
    strncpy(node->name,name,strlen(name));
    node->len = 0;
    node->BufferSize = pingpongbuffsize;
    
    return node;
}

void notifybroker_pushback(DataCentor_Header *head,const char* id)
{
     Broker_Node *newnode=NULL;
     Broker_Node *start=NULL;
     newnode = notifybroker_creatnode(head,id);
     if(newnode == NULL)
     {
          LOGGER_ERROR("notifybroker_creatnode errror \n");
          return;
     }
     if(head == NULL){
        return;
     }
     start=head->next;
	   while(start)//指针变量start的指针域若为NULL,则当前结点为末结点，循环终止
	   {
		    start=start->next;//结点向后遍历
	   }
	   start=newnode;//末结点指针域指向新结点
     head->len++;
}




/**
  * @brief  在数据中心找相关的订阅者
  * @param  
  * @param  id:  Account ID
  * @retval If the search is successful, return the pointer of the account
  */
Broker_Node* notifybroker_Find(DataCentor_Header *head,const char* id)
{
   int i = 0;
   Broker_Node *start=head->next;
   while(start)
   {
       if (strcmp(id,start->ID) == 0)
       {
          return start;
       }
       start=start->next;//结点向后遍历
   }
  LOGGER_INFO("NO FIND ID \n");
   return NULL;
}

/**
  * @brief  在初始化broker
  * @param  name:  Account ID
  * @retval If the search is successful, return the pointer of the account
  */

DataCentor_Header* notifybroker_init(const char* master,UINT32 pingpongbuffsize)
{
     DataCentor_Header *node=NULL;
     node = notifybroker_initdatacentor(master,pingpongbuffsize);
     return node;
}

/**@fn         notification_subscribe  
 * @brief      初始化参数
 * @param[in]  pINotification   pINotification对象操作指针
 * @return     成功返回OK     失败返回错误码
 */
int notifybroker_remove(DataCentor_Header *head,const char* id)
{
    Broker_Node *prestart=NULL;
    Broker_Node *start=head->next;
    while(start)
    {
       prestart=start;
       if (strcmp(id,start->ID) == 0)
       {
         start=start->next;
         prestart->next = start->next;
         sys_mem_free(prestart);
         head->len--;
         return OK;
       }
       start=start->next;//结点向后遍历
   }
   LOGGER_INFO("NO remove FIND ID \n");
   return ERROR;
}



/**@fn         notifybroker_delete  
 * @brief      删除所有的数据中心
 * @param[in]  master   数据中心名
 * @return     成功返回OK     失败返回错误码
 */
void notifybroker_delete(DataCentor_Header *head)
{
    Broker_Node *prestart=NULL;
    Broker_Node *start=head->next;
    while(start)
    {
       prestart=start;
       start=start->next;
       prestart->next = start->next;
       sys_mem_free(prestart);
       start=start->next;//结点向后遍历
    }
    sys_mem_free(head);
}