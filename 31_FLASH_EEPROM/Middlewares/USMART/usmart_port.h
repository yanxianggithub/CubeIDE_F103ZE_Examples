/**
 ****************************************************************************************************
 * V3.4之前版本详细修改说明见USMART文件夹下的:readme.txt
 * 
 * V3.4 20200324
 * 1, 新增usmart_port.c和usmart_port.h,用于管理USMART的移植,方便修改
 * 2, 修改变量命名方式为: uint8_t, uint16_t, uint32_t
 * 3, 修改usmart_reset_runtime为usmart_timx_reset_time
 * 4, 修改usmart_get_runtime为usmart_timx_get_time
 * 5, 修改usmart_scan函数实现方式,改成由usmart_get_input_string获取数据流
 * 6, 修改printf函数为USMART_PRINTF宏定义
 * 7, 修改定时扫描相关函数,改用宏定义方式,方便移植
 *
 * V3.5 20201220
 * 1，修改部分代码以支持AC6编译器
 *
 ****************************************************************************************************
 */
 
#ifndef __USMART_PORT_H
#define __USMART_PORT_H

#include "../../SYSTEM/sys/sys.h"
#include "usart.h"




/******************************************************************************************/
/* 用户配置参数 */


#define MAX_FNAME_LEN           30      /* 函数名最大长度，应该设置为不小于最长函数名的长度。 */
#define MAX_PARM                10      /* 最大为10个参数 ,修改此参数,必须修改usmart_exe与之对应. */
#define PARM_LEN                200     /* 所有参数之和的长度不超过PARM_LEN个字节,注意串口接收部分要与之对应(不小于PARM_LEN) */


#define USMART_ENTIMX_SCAN      1       /* 使用TIM的定时中断来扫描SCAN函数,如果设置为0,需要自己实现隔一段时间扫描一次scan函数.
                                         * 注意:如果要用runtime统计功能,必须设置USMART_ENTIMX_SCAN为1!!!!
                                         */

#define USMART_USE_HELP         1       /* 使用帮助，该值设为0，可以节省近700个字节，但是将导致无法显示帮助信息。 */
#define USMART_USE_WRFUNS       1       /* 使用读写函数,使能这里,可以读取任何地址的值,还可以写寄存器的值. */

#define USMART_PRINTF           printf  /* 定义printf输出 */

/******************************************************************************************/

char * usmart_get_input_string(void); 		/* 获取输入数据流 */
void usmart_timx_reset_time(void);      	/* 复位运行时间 */
uint32_t usmart_timx_get_time(void);     	/* 获取运行时间 */
void usmart_timx_init(void);   				/* 初始化定时器 */

#endif



























