
#include "exit.h"
#include "../../SYSTEM/delay/delay.h"
#include "../KEY/key.h"
#include "../LED/led.h"
#include "../BEEP/beep.h"

/**
 * @brief       中断服务程序中需要做的事情
                在HAL库中所有的外部中断服务函数都会调用此函数
 * @param       GPIO_Pin:中断引脚号
 * @retval      无
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    delay_ms(20);      /* 消抖 */
    switch(GPIO_Pin)
    {
        case KEY0_Pin:
            if (KEY0 == 0)
            {
                LED0_TOGGLE();  /* LED0 状态取反 */
                LED1_TOGGLE();  /* LED1 状态取反 */
            }
            break;
        case KEY1_Pin:
            if (KEY1 == 0)
            {
                LED0_TOGGLE();  /* LED0 状态取反 */
            }
            break;
        case WK_UP_Pin:
            if (WK_UP == 1)
            {
                BEEP_TOGGLE();  /* 蜂鸣器状态取反 */
            }
            break;
    }
}
