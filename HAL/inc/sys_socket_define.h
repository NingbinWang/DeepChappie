/**@file    sys_socket_defines.h
 * @brief   套接字的定义适配
 *
 */
#ifndef _SYS_SOCKET_DEFINE_H_
#define _SYS_SOCKET_DEFINE_H_


/* Define some BSD protocol constants.  */
#define SYS_SOCK_STREAM  1                       /* TCP Socket                                                          */
#define SYS_SOCK_DGRAM   2                       /* UDP Socket                                                          */

#define SYS_IPPROTO_IP 0  /* dummy for IP */
#define SYS_IPPROTO_ICMP 1 /* control message protocol */
#define SYS_IPPROTO_IGMP 2 /* internet group management protocol */
#define SYS_IPPROTO_GGP 3  /* gateway^2 (deprecated) */
#define SYS_IPPROTO_TCP 6  /* tcp */
#define SYS_IPPROTO_PUP 12 /* pup */
#define SYS_IPPROTO_UDP 17 /* user datagram protocol */
#define SYS_IPPROTO_IDP 22 /* xns idp */
#define SYS_IPPROTO_IPV6 41  /* IPv6-in-IPv4 tunnelling */

#define SYS_IPPROTO_ND 77   /* UNOFFICIAL net disk proto */
#define SYS_IPPROTO_RAW 255 /* raw IP packet */
#define SYS_IPPROTO_MAX 256

#define SYS_IP_TOS          1
#define SYS_IP_TTL          2
#define SYS_IP_HDRINCL      3
#define SYS_IP_OPTIONS      4
#define SYS_IP_ROUTER_ALERT 5
#define SYS_IP_RECVOPTS     6
#define SYS_IP_RETOPTS      7
#define SYS_IP_PKTINFO      8
#define SYS_IP_PKTOPTIONS   9
#define SYS_IP_MTU_DISCOVER 10
#define SYS_IP_RECVERR      11
#define SYS_IP_RECVTTL      12
#define SYS_IP_RECVTOS      13
#define SYS_IP_MTU          14
#define SYS_IP_FREEBIND     15
#define SYS_IP_IPSEC_POLICY 16
#define SYS_IP_XFRM_POLICY  17
#define SYS_IP_PASSSEC      18
#define SYS_IP_TRANSPARENT  19
#define SYS_IP_MULTICAST_IF                 32
#define SYS_IP_MULTICAST_TTL                33
#define SYS_IP_MULTICAST_LOOP               34
#define SYS_IP_ADD_MEMBERSHIP               35
#define SYS_IP_DROP_MEMBERSHIP              36
#define SYS_IP_UNBLOCK_SOURCE               37
#define SYS_IP_BLOCK_SOURCE                 38
#define SYS_IP_ADD_SOURCE_MEMBERSHIP        39
#define SYS_IP_DROP_SOURCE_MEMBERSHIP       40
#define SYS_IP_MSFILTER                     41

/* IPV6 */
#define SYS_IPV6_MULTICAST_IF                 17
#define SYS_IPV6_MULTICAST_TTL                18
#define SYS_IPV6_MULTICAST_LOOP               19
#define SYS_IPV6_ADD_MEMBERSHIP               20
#define SYS_IPV6_DROP_MEMBERSHIP              21

#define SYS_TCP_MAXSEG                      50

#define SYS_PF_INET         2       /* IP protocol family.  */
#define SYS_PF_INET6        10      /* IP version 6.  */

#define SYS_AF_INET         SYS_PF_INET
#define SYS_AF_INET6        SYS_PF_INET6                 
#define SYS_SOL_SOCKET      1
#define SYS_SO_REUSEADDR    4 /* Allow local address reuse */
#define SYS_SO_BROADCAST    6
#define SYS_SO_RCVTIMEO     20
#define SYS_SO_SNDTIMEO     21
#define SYS_SO_LINGER       22
#define SYS_SO_NO_CHECK     23
#define F_GETFL                             3                       /* Get file descriptors                                                 */
#define F_SETFL                             4                       /* Set a subset of file descriptors (e.g. O_NONBlOCK                    */
#ifndef O_NONBLOCK
#define O_NONBLOCK                          0x4000                  /* Option to enable non blocking on a file (e.g. socket)                */
#endif

#define SYS_F_GETFL                             3
#define SYS_F_SETFL                             4 
#define SYS_O_NONBLOCK                          1

#define SYS_TCP_NODELAY  0x01

#define SYS_FD_SETSIZE    1024
#define SYS_FD_SET(n, p)  ((p)->fd_bits[(n)/8u] |=  (1u << ((n) & 7u)))
#define SYS_FD_CLR(n, p)  ((p)->fd_bits[(n)/8u] &= ~(1u << ((n) & 7u)))
#define SYS_FD_ISSET(n,p) ((p)->fd_bits[(n)/8u] &   (1u << ((n) & 7u)))
#define SYS_FD_ZERO(p)    memset((void*)(p),0,sizeof(*(p)))
enum
{
  SYS_TCP_ESTABLISHED = 1,
  SYS_TCP_SYN_SENT,
  SYS_TCP_SYN_RECV,
  SYS_TCP_FIN_WAIT1,
  SYS_TCP_FIN_WAIT2,
  SYS_TCP_TIME_WAIT,
  SYS_TCP_CLOSE,
  SYS_TCP_CLOSE_WAIT,
  SYS_TCP_LAST_ACK,
  SYS_TCP_LISTEN,
  SYS_TCP_CLOSING   /* now a valid state */
}SYS_TCP_STATE;

typedef struct sys_in_addr sys_in_addr;

struct sys_in_addr
{
    UINT32           s_addr;             /* Internet address (32 bits).                         */        
};

struct sys_sockaddr_in
{
    UINT16              sin_family;         /* Internet Protocol (AF_INET).                    */
    UINT16              sin_port;           /* Address port (16 bits).                         */
    struct sys_in_addr  sin_addr;           /* Internet address (32 bits).                     */
    UINT8               sin_zero[8];        /* Not used.                                       */
};

struct sys_sockaddr
{
    UINT16          sa_family;              /* Address family (e.g. , AF_INET).                 */
    UINT8           sa_data[14];            /* Protocol- specific address information.          */
};

struct sys_linger
{
    INT32 l_onoff;                            /* 0 = disabled; 1 = enabled; default = 0;*/
    INT32 l_linger;                           /* linger time in seconds; default = 0;*/
};

struct sys_ip_mreq
{
    struct sys_in_addr imr_multiaddr; //多播组的IP地址
    struct sys_in_addr imr_interface; //加入的客服端主机IP地址
};

typedef struct sys_in6_addr sys_in6_u;
typedef struct sys_sockaddr_in6 sys_sockaddr_in6;

struct sys_in6_addr{
union {
    UINT8  u6_addr8[16];
    UINT16 u6_addr16[8];
    UINT32 u6_addr32[4];
}sys_in6_u;
#define sys_s6_addr sys_in6_u.u6_addr8
#define sys_s6_addr16 sys_in6_u.u6_addr16
#define sys_s6_addr32 sys_in6_u.u6_addr32
};

struct sys_sockaddr_in6 {
    UINT16 sin6_family; /* AF_INET6 */
    UINT16 sin6_port; /* Transport layer port # */
    UINT32 sin6_flowinfo; /* IPv6 flow information */
    struct sys_in6_addr sin6_addr; /* IPv6 address */
    UINT32 sin6_scope_id; /* scope id (new in RFC2553) */
};

struct sys_ipv6_mreq
{
  struct sys_in6_addr ipv6mr_multiaddr; /*  IPv6 multicast addr */
  UINT32              ipv6mr_interface; /*  interface index, or 0 */
};

#ifndef socklen_t
#define socklen_t UINT32
#endif

typedef struct sys_address_t{
  socklen_t size;           /**< size of addr */
  union {
    struct sys_sockaddr     sa;
    struct sys_sockaddr_in  sin;
    struct sys_sockaddr_in6 sin6;
  } addr;
}sys_address_t;


#if !defined(FIONREAD) || !defined(FIONBIO)
#define IOCPARM_MASK    0x7fU           /* parameters must be < 128 bytes */
#define IOC_VOID        0x20000000UL    /* no parameters */
#define IOC_OUT         0x40000000UL    /* copy out parameters */
#define IOC_IN          0x80000000UL    /* copy in parameters */
#define IOC_INOUT       (IOC_IN|IOC_OUT)
                                        /* 0x20000000 distinguishes new &
                                           old ioctl's */
#define _IO(x,y)        ((long)(IOC_VOID|((x)<<8)|(y)))

#define _IOR(x,y,t)     ((long)(IOC_OUT|((sizeof(t)&IOCPARM_MASK)<<16)|((x)<<8)|(y)))

#define _IOW(x,y,t)     ((long)(IOC_IN|((sizeof(t)&IOCPARM_MASK)<<16)|((x)<<8)|(y)))
#endif /* !defined(FIONREAD) || !defined(FIONBIO) */

#ifndef FIONREAD
#define FIONREAD    _IOR('f', 127, unsigned long) /* get # bytes to read */
#endif
#ifndef FIONBIO
#define FIONBIO     _IOW('f', 126, unsigned long) /* set/clear non-blocking i/o */
#endif

/**@struct  SYS_SOCKET_SHUTDONW_TYPE_E 
 * @brief   关机类型
 */
typedef enum
{
	SYS_SOCKET_SHUTDONW_TYPE_INPUT, /*断开输入流or接收流*/
	SYS_SOCKET_SHUTDONW_TYPE_OUTPUT,/*断开输出流or发送流*/
	SYS_SOCKET_SHUTDONW_TYPE_BOTH,  /*同时关闭输入输出*/
}SYS_SOCKET_SHUTDONW_TYPE_E;

#endif/*__SYS_SOCKET_DEFINE_H__*/