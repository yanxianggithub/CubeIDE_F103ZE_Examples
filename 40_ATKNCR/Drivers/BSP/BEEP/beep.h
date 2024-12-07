/**
 ****************************************************************************************************
 * @file        beep.h
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2024-01-01
 * @brief       ������ ��������
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

#ifndef BEEP_BEEP_H_
#define BEEP_BEEP_H_
#include "../../SYSTEM/sys/sys.h"

/******************************************************************************************/

/* ���������� */
#define BEEP(x)         do{ x ? \
                            HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, GPIO_PIN_SET) : \
                            HAL_GPIO_WritePin(BEEP_GPIO_Port, BEEP_Pin, GPIO_PIN_RESET); \
                        }while(0)

/* BEEP״̬��ת */
#define BEEP_TOGGLE()   do{ HAL_GPIO_TogglePin(BEEP_GPIO_Port, BEEP_Pin); }while(0)     /* BEEP = !BEEP */


#endif /* BEEP_BEEP_H_ */
