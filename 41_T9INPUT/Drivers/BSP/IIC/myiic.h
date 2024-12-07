/**
 ****************************************************************************************************
 * @file        myiic.h
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2020-04-24
 * @brief       IIC ��������
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
 * V1.0 20200424
 * ��һ�η���
 *
 ****************************************************************************************************
 */
 
#ifndef __MYIIC_H
#define __MYIIC_H

#include "../../SYSTEM/sys/sys.h"

/******************************************************************************************/

/* IO���� */
#define IIC_SCL(x)        do{ x ? \
                              HAL_GPIO_WritePin(IIC_SCL_GPIO_Port, IIC_SCL_Pin, GPIO_PIN_SET) : \
                              HAL_GPIO_WritePin(IIC_SCL_GPIO_Port, IIC_SCL_Pin, GPIO_PIN_RESET); \
                          }while(0)       /* SCL */

#define IIC_SDA(x)        do{ x ? \
                              HAL_GPIO_WritePin(IIC_SDA_GPIO_Port, IIC_SDA_Pin, GPIO_PIN_SET) : \
                              HAL_GPIO_WritePin(IIC_SDA_GPIO_Port, IIC_SDA_Pin, GPIO_PIN_RESET); \
                          }while(0)       /* SDA */

#define IIC_READ_SDA     HAL_GPIO_ReadPin(IIC_SDA_GPIO_Port, IIC_SDA_Pin) /* ��ȡSDA */


/* IIC���в������� */
void iic_init(void);            /* ��ʼ��IIC��IO�� */
void iic_start(void);           /* ����IIC��ʼ�ź� */
void iic_stop(void);            /* ����IICֹͣ�ź� */
void iic_ack(void);             /* IIC����ACK�ź� */
void iic_nack(void);            /* IIC������ACK�ź� */
uint8_t iic_wait_ack(void);     /* IIC�ȴ�ACK�ź� */
void iic_send_byte(uint8_t txd);/* IIC����һ���ֽ� */
uint8_t iic_read_byte(unsigned char ack);/* IIC��ȡһ���ֽ� */

#endif

