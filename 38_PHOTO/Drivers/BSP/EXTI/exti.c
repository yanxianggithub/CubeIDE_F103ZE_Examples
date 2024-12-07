/**
 ****************************************************************************************************
 * @file        exti.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2020-04-19
 * @brief       �ⲿ�ж� ��������
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
 * V1.0 20200420
 * ��һ�η���
 * V1.1 20200426
 * 1, ����exti8_init��������Ӧ�жϷ�����
 * 2, ֧�ֶ�OV7725 VSYNC�жϵĴ���
 * 3, ���� g_ov7725_vsta ȫ�ֱ���
 *
 ****************************************************************************************************
 */

#include "../../SYSTEM/sys/sys.h"
#include "../../SYSTEM/delay/delay.h"
#include "exti.h"
#include "../../BSP/OV7725/ov7725.h"

/* OV7725 ֡�жϱ�־
 * 0, ��ʾ��һ֡�����Ѿ���������, �µ�֡�жϵ���, ������FIFOд����
 * 1, ��ʾ��һ֡���ݻ�û�д�����, �µ�֡�жϵ���, ������FIFOд����
 */
uint8_t g_ov7725_vsta = 0;

/**
 * @brief       OV7725 VSYNC �ⲿ�жϷ������
 * @param       ��
 * @retval      ��
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == OV7725_VSYNC_Pin)   /* ��OV7725_VSYNC_Pin�ߵ��ж� */
    {
        if (g_ov7725_vsta == 0) /* ��һ֡�����Ѿ�������? */
        {
            OV7725_WRST(0);     /* ��λдָ�� */
            OV7725_WRST(1);     /* ������λ */
            OV7725_WEN(1);      /* ����д��FIFO */
            g_ov7725_vsta = 1;  /* ���֡�ж� */
        }
        else
        {
            OV7725_WEN(0);      /* ��ֹд��FIFO */
        }
    }

    __HAL_GPIO_EXTI_CLEAR_IT(OV7725_VSYNC_Pin);   /* ���OV7725_VSYNC_Pin�ϵ��жϱ�־λ */
}










