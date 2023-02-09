      
/**@file app_errno.h
 * @note TUUBU System Technology Co., Ltd. All Right Reserved.
 * @brief  
 * 
 * @author   alex
 * @date     2022-08-30
 * @version  1.0
 * 
 * @note ///Description here 
 * @note History:        
 * @note     <author>   <time>    <version >   <desc>
 * @note    alex       2022-08-30  V1.0    Create
 * @warning  
 */

#ifndef _APP_ERROR_H_
#define _APP_ERROR_H_
#ifdef  __cplusplus
extern "C" {
#endif  /* __cplusplus */


#define APP_ERR_MAGIC            (0xB8)

#define APP_OK                   (0)
#define APP_ERR(main, sub, err)  ((((AT_ERR_MAGIC) << 24) & 0xff000000) | (((main) << 16) & 0x00ff0000) | (((sub) << 8) & 0x0000ff00) | ((err) & 0x000000ff))
#define APP_IS_ERR(errno)        ((((errno) & 0xff000000) == ((AT_ERR_MAGIC) << 24)) || (((errno) < 0) && ((errno) > -4096)))






#ifdef  __cplusplus
}
#endif  /* __cplusplus */

#endif