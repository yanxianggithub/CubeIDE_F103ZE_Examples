/**
 ****************************************************************************************************
 * V3.4֮ǰ�汾��ϸ�޸�˵����USMART�ļ����µ�:readme.txt
 * 
 * V3.4 20200324
 * 1, ����usmart_port.c��usmart_port.h,���ڹ���USMART����ֲ,�����޸�
 * 2, �޸ı���������ʽΪ: uint8_t, uint16_t, uint32_t
 * 3, �޸�usmart_reset_runtimeΪusmart_timx_reset_time
 * 4, �޸�usmart_get_runtimeΪusmart_timx_get_time
 * 5, �޸�usmart_scan����ʵ�ַ�ʽ,�ĳ���usmart_get_input_string��ȡ������
 * 6, �޸�printf����ΪUSMART_PRINTF�궨��
 * 7, �޸Ķ�ʱɨ����غ���,���ú궨�巽ʽ,������ֲ
 *
 * V3.5 20201220
 * 1���޸Ĳ��ִ�����֧��AC6������
 *
 ****************************************************************************************************
 */
 
#ifndef __USMART_PORT_H
#define __USMART_PORT_H

#include "../../SYSTEM/sys/sys.h"
#include "usart.h"




/******************************************************************************************/
/* �û����ò��� */


#define MAX_FNAME_LEN           30      /* ��������󳤶ȣ�Ӧ������Ϊ��С����������ĳ��ȡ� */
#define MAX_PARM                10      /* ���Ϊ10������ ,�޸Ĵ˲���,�����޸�usmart_exe��֮��Ӧ. */
#define PARM_LEN                200     /* ���в���֮�͵ĳ��Ȳ�����PARM_LEN���ֽ�,ע�⴮�ڽ��ղ���Ҫ��֮��Ӧ(��С��PARM_LEN) */


#define USMART_ENTIMX_SCAN      1       /* ʹ��TIM�Ķ�ʱ�ж���ɨ��SCAN����,�������Ϊ0,��Ҫ�Լ�ʵ�ָ�һ��ʱ��ɨ��һ��scan����.
                                         * ע��:���Ҫ��runtimeͳ�ƹ���,��������USMART_ENTIMX_SCANΪ1!!!!
                                         */

#define USMART_USE_HELP         1       /* ʹ�ð�������ֵ��Ϊ0�����Խ�ʡ��700���ֽڣ����ǽ������޷���ʾ������Ϣ�� */
#define USMART_USE_WRFUNS       1       /* ʹ�ö�д����,ʹ������,���Զ�ȡ�κε�ַ��ֵ,������д�Ĵ�����ֵ. */

#define USMART_PRINTF           printf  /* ����printf��� */

/******************************************************************************************/

char * usmart_get_input_string(void); 		/* ��ȡ���������� */
void usmart_timx_reset_time(void);      	/* ��λ����ʱ�� */
uint32_t usmart_timx_get_time(void);     	/* ��ȡ����ʱ�� */
void usmart_timx_init(void);   				/* ��ʼ����ʱ�� */

#endif



























