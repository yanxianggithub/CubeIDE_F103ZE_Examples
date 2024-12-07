/**
 ****************************************************************************************************
 * @file        sccb.h
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2020-04-26
 * @brief       SCCB ��������
 * @license     Copyright (c) 2020-2032, �������������ӿƼ����޹�˾
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
 * V1.0 20200426
 * ��һ�η���
 *
 ****************************************************************************************************
 */

#ifndef __SCCB_H
#define __SCCB_H

#include "../../SYSTEM/sys/sys.h"

/******************************************************************************************/
#define OV_SCCB_TYPE_NOD     0        /* ����ò��ÿ�©ģʽ��SCCB�����������裬������Ҫ����������л��ķ�ʽ */
#if OV_SCCB_TYPE_NOD
    #define SCCB_SDA_IN()  {GPIOG->CRH&=0XFF0FFFFF;GPIOG->CRH|=0X00800000;}
    #define SCCB_SDA_OUT() {GPIOG->CRH&=0XFF0FFFFF;GPIOG->CRH|=0X00300000;}
#endif
/******************************************************************************************/

/* IO�������� */
#define SCCB_SCL(x)   do{ x ? \
                          HAL_GPIO_WritePin(SCCB_SCL_GPIO_Port, SCCB_SCL_Pin, GPIO_PIN_SET) : \
                          HAL_GPIO_WritePin(SCCB_SCL_GPIO_Port, SCCB_SCL_Pin, GPIO_PIN_RESET); \
                      }while(0)  /* SCL */

#define SCCB_SDA(x)   do{ x ? \
                          HAL_GPIO_WritePin(SCCB_SDA_GPIO_Port, SCCB_SDA_Pin, GPIO_PIN_SET) : \
                          HAL_GPIO_WritePin(SCCB_SDA_GPIO_Port, SCCB_SDA_Pin, GPIO_PIN_RESET); \
                      }while(0)  /* SDA */

                      
#define SCCB_READ_SDA       HAL_GPIO_ReadPin(SCCB_SDA_GPIO_Port, SCCB_SDA_Pin)     /* ��ȡSDA */


/* ����ӿں��� */
void sccb_init(void);
void sccb_stop(void);
void sccb_start(void);

void sccb_nack(void);
uint8_t sccb_read_byte(void);
uint8_t sccb_send_byte(uint8_t data);

#endif












