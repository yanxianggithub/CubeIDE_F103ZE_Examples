/**
 ****************************************************************************************************
 * @file        led.h
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2024-01-01
 * @brief       LED ��������
 * @license     Copyright (c) 2020-2032, ������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:����ԭ�� STM32F103������
 * ������Ƶ:www.yuanzige.com
 * ������̳:www.openedv.com
 * ��˾��ַ:www.alientek.com
 * �����ַ:openedv.taobao.com
 *
 * �޸�˵��
 * V1.0 20240101
 * ��һ�η���
 *
 ****************************************************************************************************
 */

#ifndef LED_H_
#define LED_H_
#include "../../SYSTEM/sys/sys.h"

/******************************************************************************************/
/* LED�˿ڶ��� */
#define LED0(x)   do{ x ? \
                      HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_SET) : \
                      HAL_GPIO_WritePin(LED0_GPIO_Port, LED0_Pin, GPIO_PIN_RESET); \
                  }while(0)      /* LED0��ת */

#define LED1(x)   do{ x ? \
                      HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_SET) : \
                      HAL_GPIO_WritePin(LED1_GPIO_Port, LED1_Pin, GPIO_PIN_RESET); \
                  }while(0)      /* LED1��ת */

/* LEDȡ������ */
#define LED0_TOGGLE()   do{ HAL_GPIO_TogglePin(LED0_GPIO_Port, LED0_Pin); }while(0)        /* ��תLED0 */
#define LED1_TOGGLE()   do{ HAL_GPIO_TogglePin(LED1_GPIO_Port, LED1_Pin); }while(0)        /* ��תLED1 */

#endif /* LED_H_ */
