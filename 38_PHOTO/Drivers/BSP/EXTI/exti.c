/**
 ****************************************************************************************************
 * @file        exti.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2020-04-19
 * @brief       外部中断 驱动代码
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 STM32F103开发板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
 *
 * 修改说明
 * V1.0 20200420
 * 第一次发布
 * V1.1 20200426
 * 1, 增加exti8_init函数及对应中断服务函数
 * 2, 支持对OV7725 VSYNC中断的处理
 * 3, 增加 g_ov7725_vsta 全局变量
 *
 ****************************************************************************************************
 */

#include "../../SYSTEM/sys/sys.h"
#include "../../SYSTEM/delay/delay.h"
#include "exti.h"
#include "../../BSP/OV7725/ov7725.h"

/* OV7725 帧中断标志
 * 0, 表示上一帧数据已经处理完了, 新的帧中断到来, 可以往FIFO写数据
 * 1, 表示上一帧数据还没有处理完, 新的帧中断到来, 不能往FIFO写数据
 */
uint8_t g_ov7725_vsta = 0;

/**
 * @brief       OV7725 VSYNC 外部中断服务程序
 * @param       无
 * @retval      无
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == OV7725_VSYNC_Pin)   /* 是OV7725_VSYNC_Pin线的中断 */
    {
        if (g_ov7725_vsta == 0) /* 上一帧数据已经处理了? */
        {
            OV7725_WRST(0);     /* 复位写指针 */
            OV7725_WRST(1);     /* 结束复位 */
            OV7725_WEN(1);      /* 允许写入FIFO */
            g_ov7725_vsta = 1;  /* 标记帧中断 */
        }
        else
        {
            OV7725_WEN(0);      /* 禁止写入FIFO */
        }
    }

    __HAL_GPIO_EXTI_CLEAR_IT(OV7725_VSYNC_Pin);   /* 清除OV7725_VSYNC_Pin上的中断标志位 */
}










