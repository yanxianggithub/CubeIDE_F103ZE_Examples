
#ifndef __PWR_H
#define __PWR_H

#include "../../SYSTEM/sys/sys.h"

/******************************************************************************************/

void pwr_pvd_init(uint32_t pls); /* PVD��ѹ����ʼ������ */
void pwr_wkup_key_init(void);    /* ���Ѱ�����ʼ�� */
void pwr_enter_sleep(void);      /* ����˯��ģʽ */

#endif




















