#ifndef _MEDIA_TYPE_H_
#define _MEDIA_TYPE_H_

/*fixed for enum type*/
#ifndef INT_MINI
#define INT_MINI     (-2147483647 - 1) /* minimum (signed) int value */
#endif

#ifndef INT_MAXI
#define INT_MAXI      (2147483647)    /* maximum (signed) int value */
#endif


#define MEDIA_START_ERR                     (-1u)
#define MEDIA_INIT_ERR                      (-2u)
#define MEDIA_MEM_ERR                       (-3u)

#define MEDIA_OK						(0)
#define MEDIA_ERR_INVALID_CHAN			(1)
#define MEDIA_ERR_INVALID_PARAM		(2)
#define MEDIA_ERR_INVALID_ADDR			(3)
#define MEDIA_ERR_TIMEOUT				(4)
#define MEDIA_ERR_NOT_ENOUGH_MEM		(5)
#define MEDIA_ERR_HARDWARE_NOT_EXIST	(6)
#define MEDIA_ERR_BUSY					(7)
#define MEDIA_ERR_UNSUPPORTED			(8)
#define MEDIA_ERR_UNDEFINED			(0x7fffffff)


typedef unsigned long long      UINT64;
typedef signed long long        INT64;
typedef unsigned long           UINT32;
typedef signed long             INT32;
typedef unsigned short          UINT16;
typedef signed short            INT16;
typedef unsigned char           UINT8;
typedef signed char             INT8;
typedef unsigned int            UINT;
typedef signed int              INT;
typedef float                   FLOAT;
typedef double                  DOUBLE;
typedef char                    CHAR;
typedef unsigned int            BOOL;




#endif