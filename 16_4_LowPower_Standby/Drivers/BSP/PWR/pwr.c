
#include "pwr.h"
#include "../LED/led.h"
#include "../LCD/lcd.h"


/**
 * @brief       ��ʼ��PVD��ѹ������
 * @param       pls: ��ѹ�ȼ�(PWR_PVD_detection_level)
 *   @arg       PWR_PVDLEVEL_0,2.2V;
 *   @arg       PWR_PVDLEVEL_1,2.3V;
 *   @arg       PWR_PVDLEVEL_2,2.4V;
 *   @arg       PWR_PVDLEVEL_3,2.5V;
 *   @arg       PWR_PVDLEVEL_4,2.6V;
 *   @arg       PWR_PVDLEVEL_5,2.7V;
 *   @arg       PWR_PVDLEVEL_6,2.8V;
 *   @arg       PWR_PVDLEVEL_7,2.9V;
 * @retval      ��
 */
void pwr_pvd_init(uint32_t pls)
{
    PWR_PVDTypeDef pwr_pvd = {0};

    __HAL_RCC_PWR_CLK_ENABLE();                      /* ʹ��PWRʱ�� */
    
    pwr_pvd.PVDLevel = pls;                          /* ����ѹ���� */
    pwr_pvd.Mode = PWR_PVD_MODE_IT_RISING_FALLING;   /* ʹ���ж��ߵ������غ��½���˫��Ե���� */
    HAL_PWR_ConfigPVD(&pwr_pvd);

    HAL_NVIC_SetPriority(PVD_IRQn, 3 ,3);
    HAL_NVIC_EnableIRQ(PVD_IRQn);
    HAL_PWR_EnablePVD();                             /* ʹ��PVD��� */
}

/**
 * @brief       PVD�жϷ�����
 * @param       ��
 * @retval      ��
 */
void PVD_IRQHandler(void)
{
    HAL_PWR_PVD_IRQHandler();
}

/**
 * @brief       PVD�жϷ���ص�����
 * @param       ��
 * @retval      ��
 */
void HAL_PWR_PVDCallback(void)
{
    if (__HAL_PWR_GET_FLAG(PWR_FLAG_PVDO))   /* ��ѹ��PLS��ѡ��ѹ���� */
    {
        lcd_show_string(30, 130, 200, 16, 16, "PVD Low Voltage!", RED); /* LCD��ʾ��ѹ�� */
        LED1(0);                                                        /* �����̵�, ������ѹ���� */
    }
    else
    {
        lcd_show_string(30, 130, 200, 16, 16, "PVD Voltage OK! ", BLUE);/* LCD��ʾ��ѹ���� */
        LED1(1);                                                        /* ����̵� */
    }
}

/***********************************˯��ģʽʵ�����*******************************************/

/**
 * @brief       ����CPU˯��ģʽ
 * @param       ��
 * @retval      ��
 */
void pwr_enter_sleep(void)
{
    HAL_PWR_EnterSLEEPMode(PWR_MAINREGULATOR_ON, PWR_SLEEPENTRY_WFI); /* ִ��WFIָ��, ����˯��ģʽ */
}

/**
 * @brief       �ⲿ�жϻص�����
 * @param       GPIO_Pin:�ж�������
 * @note        �˺����ᱻPWR_WKUP_INT_IRQHandler()����
 * @retval      ��
 */
void HAL_GPIO_EXTI_Callback(uint16_t GPIO_Pin)
{
    if (GPIO_Pin == WK_UP_Pin)
    {
        /* HAL_GPIO_EXTI_IRQHandler()�����Ѿ�Ϊ����������жϱ�־λ���������ǽ��˻ص��������Բ����κ��� */
    }
}

/***********************************ֹͣģʽʵ�����*******************************************/

/**
 * @brief       ����ֹͣģʽ
 * @param       ��
 * @retval      ��
 */
void pwr_enter_stop(void)
{
    __HAL_RCC_PWR_CLK_ENABLE();     /* ʹ�ܵ�Դʱ�� */

    /* ����ֹͣģʽ��������ѹ��Ϊ�͹���ģʽ���ȴ��жϻ��� */
    HAL_PWR_EnterSTOPMode(PWR_LOWPOWERREGULATOR_ON, PWR_STOPENTRY_WFI);
}

/***********************************����ģʽ*******************************************/
/**
 * @brief       �������ģʽ
 * @param       ��
 * @retval      ��
 */
void pwr_enter_standby(void)
{
    __HAL_RCC_PWR_CLK_ENABLE();               /* ʹ�ܵ�Դʱ�� */

    HAL_PWR_EnableWakeUpPin(PWR_WAKEUP_PIN1); /* ʹ��KEY_UP���ŵĻ��ѹ��� */
    __HAL_PWR_CLEAR_FLAG(PWR_FLAG_WU);        /* ��Ҫ��˱�ǣ����򽫱��ֻ���״̬ */
    HAL_PWR_EnterSTANDBYMode();               /* �������ģʽ */
}







