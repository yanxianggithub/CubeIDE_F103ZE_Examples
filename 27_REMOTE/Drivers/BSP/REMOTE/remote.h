
#ifndef __REMOTE_H
#define __REMOTE_H

#include "../../SYSTEM/sys/sys.h"
#include "tim.h"

/******************************************************************************************/
/* �����������ż���ʱ�� ���� */

#define REMOTE_IN_TIMX                          TIM4                       
#define REMOTE_IN_TIMX_IRQn                     TIM4_IRQn
#define REMOTE_IN_TIMX_CHY                      TIM_CHANNEL_4                               /* ͨ��Y,  1<= Y <=2*/
#define REMOTE_IN_TIMX_CCRY                     REMOTE_IN_TIMX->CCR4
#define REMOTE_IN_TIMX_CHY_CLK_ENABLE()         do{ __HAL_RCC_TIM4_CLK_ENABLE(); }while(0)  /* TIMX ʱ��ʹ�� */

/******************************************************************************************/


#define RDATA           HAL_GPIO_ReadPin(REMOTE_IN_GPIO_Port, REMOTE_IN_Pin)   /* ������������� */


/* ����ң��ʶ����(ID),ÿ��ң�����ĸ�ֵ��������һ��,��Ҳ��һ����.
 * ����ѡ�õ�ң����ʶ����Ϊ0
*/
#define REMOTE_ID       0

extern uint8_t g_remote_cnt;    /* �������µĴ��� */
    
uint8_t remote_scan(void);

#endif















