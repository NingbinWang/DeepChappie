
#ifndef _SYS_NET_PRIV_H_
#define _SYS_NET_PRIV_H_
#include <errno.h>
#include <sys/types.h>          /* See NOTES */
#include <sys/socket.h>
#include <sys/select.h>
#include <linux/if.h>   
#include <linux/if_ether.h>   
#include <sys/ioctl.h>
#include <netinet/in.h>
#include <netinet/ip.h>
#include <netinet/tcp.h>
#include <arpa/inet.h>
#include <net/if_arp.h>
#include <stdlib.h>
#include <unistd.h>
#include <net/route.h>
#include"sys_mem.h"
#include"sys_net.h"
#include"sys_utils.h"
#include"sys_common_inside.h"

#define SYS_NET_IFINET6_PATH	"/proc/net/if_inet6"

#endif