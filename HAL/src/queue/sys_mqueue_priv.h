#ifndef _SYS_MQUEUE_PRIV_H_
#define _SYS_MQUEUE_PRIV_H_
#include <mqueue.h>
#include <unistd.h>
#include <sys/types.h>
#include"sys_common_inside.h"
#include "sys_mqueue.h"

#define MSG_Q_MODE          (0666)
#define MSG_Q_PRIO          (0)
#define MSG_Q_OFLAG         (O_CREAT|O_RDWR|O_EXCL)

typedef struct mq_attr  MSG_Q_ATTR;

#endif