#ifndef _APP_MACRO_H_
#define _APP_MACRO_H_

#ifdef  __cplusplus
extern "C" {
#endif  /* __cplusplus */

#define app_round_up(x, align)      ((((long)(x) + ((align) - 1))/(align))*(align))
#define app_round_down(x, align)    (((long)(x)/(align))*(align))

#define app_min(a,b) ((a)<(b)?(a):(b))
#define app_max(a,b) ((a)>(b)?(a):(b))

#define app_min_t(type,a,b) ({  \
	type __min1 = (a);			\
	type __min2 = (b);			\
	__min1 < __min2 ? __min1: __min2; })

#define app_max_t(type,a,b) ({  \
	type __max1 = (a);			\
	type __max2 = (b);			\
	__max1 < __max2 ? __max1: __max2; })


#define app_array_num(array)     (sizeof(array)/sizeof(array[0]))

#define APP_BIT(nr)              (1UL << (nr))
#define APP_ARRAY_NUM(arr)       (sizeof(arr) / sizeof((arr)[0]))

#define APP_RET_RETURN(cond, ret, ...)   \
    if (cond) { \
        printf("[%s:%d]RETURN:", __FUNCTION__, __LINE__); \
        printf(__VA_ARGS__); \
        printf("\n"); \
        return ret; \
    }

#define APP_RET_RETURN_NORET(cond, ...)   \
    if (cond) { \
        printf("[%s:%d]RETURN:", __FUNCTION__, __LINE__); \
        printf(__VA_ARGS__); \
        printf("\n"); \
        return; \
    }

#define APP_RET_GOTO(cond, label, ...)   \
    if (cond) { \
        printf("[%s:%d]GOTO:", __FUNCTION__, __LINE__); \
        printf(__VA_ARGS__); \
        printf("\n"); \
        goto label; \
    }

#define APP_RET_WARN(cond, ...)   \
    if (cond) { \
        printf("[%s:%d]WARN:", __FUNCTION__, __LINE__); \
        printf(__VA_ARGS__); \
        printf("\n"); \
    }

#ifdef  __cplusplus
}
#endif  /* __cplusplus */

#endif  /*_AT_MACRO_H_*/
