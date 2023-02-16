#ifndef _SYS_BLOCK_PRIV_H_
#define _SYS_BLOCK_PRIV_H_

#include <stdio.h>
#include <stdlib.h>
#include <string.h>
#include <sys/types.h>
#include <sys/stat.h>
#include <fcntl.h>
#include <unistd.h>
#include <sys/mman.h>
#include <errno.h>
#include <sys/ioctl.h>
#include <sys/prctl.h>
#include <dirent.h>
#include <ctype.h>
#include <mtd/mtd-user.h>
#include "sys_common_inside.h"
#include "sys_mem.h"


#define SYS_BLOCK_TMPLEN     (0x40000)

#ifndef BLKGETSIZE64
#define BLKGETSIZE64 _IOR(0x12,114,size_t)
#endif


#endif