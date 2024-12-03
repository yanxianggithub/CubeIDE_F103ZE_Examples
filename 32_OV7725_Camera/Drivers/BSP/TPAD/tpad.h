
#ifndef TPAD_TPAD_H_
#define TPAD_TPAD_H_
#include "../../SYSTEM/sys/sys.h"

#define TPAD_TIMX_CAP_CHY                       TIM_CHANNEL_2 		/* ͨ��Y,  1<= Y <=4 */
/******************************************************************************************/
/* ����������ֵ, Ҳ���Ǳ������ g_tpad_default_val + TPAD_GATE_VAL
 * ����Ϊ����Ч����, �Ĵ� TPAD_GATE_VAL, ���Խ���������, ��֮, ��������������
 * ����ʵ������, ѡ����ʵ� TPAD_GATE_VAL ����
 */
#define TPAD_GATE_VAL       100             	/* ����������ֵ, Ҳ���Ǳ������ g_tpad_default_val + TPAD_GATE_VAL, ����Ϊ����Ч���� */
#define TPAD_ARR_MAX_VAL    0XFFFF          	/* ����ARRֵ, һ������Ϊ��ʱ����ARR���ֵ */
extern volatile uint16_t 	g_tpad_default_val;	/* ���ص�ʱ��(û���ְ���),��������Ҫ��ʱ�� */


/* �ӿں���, ����������.c���� */
uint8_t tpad_init(void);    		/* TPAD ��ʼ�� ���� */
uint8_t tpad_scan(uint8_t mode);    /* TPAD ɨ�� ���� */

#endif /* TPAD_TPAD_H_ */
