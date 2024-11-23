
#include "exit.h"
#include "../../SYSTEM/delay/delay.h"
#include "../KEY/key.h"
#include "../LED/led.h"
#include "../BEEP/beep.h"

/**
 * @brief       �жϷ����������Ҫ��������
                ��HAL�������е��ⲿ�жϷ�����������ô˺���
 * @param       GPIO_Pin:�ж����ź�
 * @retval      ��
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    delay_ms(20);      /* ���� */
    switch(GPIO_Pin)
    {
        case KEY0_Pin:
            if (KEY0 == 0)
            {
                LED0_TOGGLE();  /* LED0 ״̬ȡ�� */
                LED1_TOGGLE();  /* LED1 ״̬ȡ�� */
            }
            break;
        case KEY1_Pin:
            if (KEY1 == 0)
            {
                LED0_TOGGLE();  /* LED0 ״̬ȡ�� */
            }
            break;
        case WK_UP_Pin:
            if (WK_UP == 1)
            {
                BEEP_TOGGLE();  /* ������״̬ȡ�� */
            }
            break;
    }
}
