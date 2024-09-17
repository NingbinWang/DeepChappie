#include <stdio.h>
#include <stdlib.h>
#include <unistd.h>           // close()
#include <string.h>           // strcpy, memset(), and memcpy()
#include <sys/types.h>        // needed for socket(), uint8_t, uint16_t, uint32_t
#include <sys/socket.h>       // needed for socket()
#include <sys/ioctl.h>        // macro ioctl is defined

#include <dirent.h>

#include <sys/stat.h>
#include <fcntl.h>

#include <errno.h>            // errno, perror()

#include <linux/i2c.h>
#include <linux/i2c-dev.h>
int i2c_devopen(int i2cbus)
{
    int fd = -1;
    char filename[128];
    sprintf(filename,"/dev/i2c-%d",i2cbus);
    fd = open(filename,O_RDWR);
    if(fd < 0)
    {
        printf("i2c_devopen fail i2cdev fd = %d\n",fd);
    }
    return 0;
}

int i2c_devioctl(int fd,int force,unsigned int address)
{
    int ret = -1;
    ret = ioctl(fd,force?I2C_SLAVE_FORCE:I2C_SLAVE,address);
    if (ret < 0)
    {
        fprintf(stderr,"ERROR:could not set address to 0x%02x: %s\n",address,strerror(errno));
        return -errno;
    }
    return 0;
}

int i2c_read_bytes(int fd,int slave_addr,unsigned int reg_addr,unsigned char * values,unsigned int size)
{
    unsigned char outbuf[1];
    int ret = -1;
    struct i2c_rdwr_ioctl_data packet;
    struct i2c_msg messages[2];
    outbuf[0] = reg_addr;
    messages[0].addr = slave_addr;
    messages[0].flags = 0;
    messages[0].len = sizeof(outbuf);
    messages[0].buf = outbuf;

    messages[1].addr = slave_addr;
    messages[1].flags = I2C_M_RD;
    messages[1].len = size;
    messages[1].buf = values;

    packet.msgs = messages;
    packet.nmsgs = 2;
    ret = ioctl(fd,I2C_RDWR,&packet);
    if(ret < 0 )
    {
        fprintf(stderr,"ERROR: I2C RD fail to 0x%02x: %s\n",slave_addr,strerror(errno));
        return -errno;
    }
    return 0;
}