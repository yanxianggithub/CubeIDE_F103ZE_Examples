
#ifndef __PWR_H
#define __PWR_H

#include "../../SYSTEM/sys/sys.h"

/******************************************************************************************/

void pwr_pvd_init(uint32_t pls); /* PVD电压检测初始化函数 */
void pwr_wkup_key_init(void);    /* 唤醒按键初始化 */
void pwr_enter_sleep(void);      /* 进入睡眠模式 */

#endif




















