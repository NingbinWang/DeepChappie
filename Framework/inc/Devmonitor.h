#ifndef  _DEVMONITOR_H_
#define  _DEVMONITOR_H_

#ifdef __cplusplus
extern "C" {
#endif/*__cplusplus*/

/**@enum  DEV_MONITOR_TYPE_E
 * @brief 分区类型
 */
typedef enum
{
    DEV_MONITOR_TYPE_INVAILD   = 0x00,/*无效值*/
    DEV_MONITOR_TYPE_TF        = 0x01,/*监听卡介质*/
    DEV_MONITOR_TYPE_HARD_DISK = 0x02,/*监听硬盘介质*/ 
    DEV_MONITOR_TYPE_WIFI      = 0x03,/*监听wifi设备*/
    DEV_MONITOR_TYPE_WIRELESS  = 0x04,/*监听无线设备 4G/5G*/
}DEV_MONITOR_TYPE_E;

/**@enum  DEV_MONITOR_ACTION_E
 * @brief 节点动作
 */
typedef enum
{
    DEV_MONITOR_ACTION_INVAILD    = 0x00,/*无效值*/
    DEV_MONITOR_ACTION_REMOVE     = 0x01,/*设备节点移除*/ 
    DEV_MONITOR_ACTION_ADD        = 0x02,/*设备节点插入*/
    DEV_MONITOR_ACTION_CHANGE     = 0x03,/*设备节点改变*/
}DEV_MONITOR_ACTION_E;

/**@fn         RegisterActionCallBack      
 * @brief      注册设备监听回调函数
 * @param[in]  eType         设备监听类型
 * @param[in]  iDevIndex     设备序列号索引
 * @param[in]  iPartIndex    子序号索引，0为无效值
 * @param[in]  eAction       设备动作
 * @return     无
 */
typedef void (*DevActionCallBack)(DEV_MONITOR_TYPE_E eType, int iDevIndex, int iPartIndex, DEV_MONITOR_ACTION_E eAction);

typedef struct IDevMonitor IDevMonitor;

struct IDevMonitor
{
    /**@fn         RegisterActionCallBack      
     * @brief      注册设备监听回调函数
     * @param[in]  pIDevMonitor  设备监听对象
     * @param[in]  eType         设备监听类型
     * @param[in]  pCallBack     回调函数
     * @return     无
     */
    void (*RegisterActionCallBack)(IDevMonitor *pIDevMonitor, DEV_MONITOR_TYPE_E eType, DevActionCallBack pCallBack);

    /**@fn         CheckDev      
     * @brief      设备节点检测
     * @param[in]  pIDevMonitor  设备监听对象
     * @param[in]  eType         设备监听类型
     * @param[in]  iDevIndex     设备节点索引
     * @return     无
     */
    int (*CheckDev)(IDevMonitor *pIDevMonitor,DEV_MONITOR_TYPE_E eType, int iDevIndex);
};

IDevMonitor *devmonitor_init_instance(VOID);

#ifdef __cplusplus
}
#endif/*__cplusplus*/

#endif