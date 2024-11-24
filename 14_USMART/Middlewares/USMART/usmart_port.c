
#include "usmart.h"
#include "usmart_port.h"
#include "tim.h"

/**
 * @brief       获取输入数据流(字符串)
 *   @note      USMART通过解析该函数返回的字符串以获取函数名及参数等信息
 * @param       无
 * @retval
 *   @arg       0,  没有接收到数据
 *   @arg       其他,数据流首地址(不能是0)
 */
char *usmart_get_input_string(void)
{
    uint8_t len;
    char *pbuf = 0;

    if (g_usart_rx_sta & 0x8000)        /* 串口接收完成？ */
    {
        len = g_usart_rx_sta & 0x3fff;  /* 得到此次接收到的数据长度 */
        g_usart_rx_buf[len] = '\0';     /* 在末尾加入结束符. */
        pbuf = (char*)g_usart_rx_buf;
        g_usart_rx_sta = 0;             /* 开启下一次接收 */
    }

    return pbuf;
}

/* 如果使能了定时器扫描, 则需要定义如下函数 */
#if USMART_ENTIMX_SCAN == 1

/**
 * 移植注意:本例是以stm32为例,如果要移植到其他mcu,请做相应修改.
 * usmart_reset_runtime,清除函数运行时间,连同定时器的计数寄存器以及标志位一起清零.并设置重装载值为最大,以最大限度的延长计时时间.
 * usmart_get_runtime,获取函数运行时间,通过读取CNT值获取,由于usmart是通过中断调用的函数,所以定时器中断不再有效,此时最大限度
 * 只能统计2次CNT的值,也就是清零后+溢出一次,当溢出超过2次,没法处理,所以最大延时,控制在:2*计数器CNT*0.1ms.对STM32来说,是:13.1s左右
 * 其他的:USMART_TIMX_IRQHandler和Timer4_Init,需要根据MCU特点自行修改.确保计数器计数频率为:10Khz即可.另外,定时器不要开启自动重装载功能!!
 */

/**
 * @brief       复位runtime
 *   @note      需要根据所移植到的MCU的定时器参数进行修改
 * @param       无
 * @retval      无
 */
void usmart_timx_reset_time(void)
{
    __HAL_TIM_CLEAR_FLAG(&htim4, TIM_FLAG_UPDATE); /* 清除中断标志位 */
    __HAL_TIM_SET_AUTORELOAD(&htim4, 0XFFFF);      /* 将重装载值设置到最大 */
    __HAL_TIM_SET_COUNTER(&htim4, 0);              /* 清空定时器的CNT */
    usmart_dev.runtime = 0;
}

/**
 * @brief       获得runtime时间
 *   @note      需要根据所移植到的MCU的定时器参数进行修改
 * @param       无
 * @retval      执行时间,单位:0.1ms,最大延时时间为定时器CNT值的2倍*0.1ms
 */
uint32_t usmart_timx_get_time(void)
{
    if (__HAL_TIM_GET_FLAG(&htim4, TIM_FLAG_UPDATE) == SET)  /* 在运行期间,产生了定时器溢出 */
    {
        usmart_dev.runtime += 0XFFFF;
    }
    usmart_dev.runtime += __HAL_TIM_GET_COUNTER(&htim4);
    return usmart_dev.runtime;                                 /* 返回计数值 */
}


/**
 * @brief       USMART定时器中断服务函数
 * @param       无
 * @retval      无
 */
void TIM4_IRQHandler(void)
{
	/* 溢出中断 */
	if(__HAL_TIM_GET_IT_SOURCE(&htim4,TIM_IT_UPDATE)==SET)
	{

		usmart_dev.scan();                              	/* 执行usmart扫描 */
		__HAL_TIM_SET_COUNTER(&htim4, 0);;    				/* 清空定时器的CNT */
		__HAL_TIM_SET_AUTORELOAD(&htim4, 100);				/* 恢复原来的设置 */
	}
	__HAL_TIM_CLEAR_IT(&htim4, TIM_IT_UPDATE);				/* 清除中断标志位 */
}

#endif
















