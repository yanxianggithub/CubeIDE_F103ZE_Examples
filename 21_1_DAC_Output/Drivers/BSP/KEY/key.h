
#ifndef KEY_KEY_H_
#define KEY_KEY_H_
#include "../../SYSTEM/sys/sys.h"

#define KEY0        HAL_GPIO_ReadPin(KEY0_GPIO_Port, KEY0_Pin)     	/* ��ȡKEY0���� */
#define KEY1        HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin)     	/* ��ȡKEY1���� */
#define WK_UP       HAL_GPIO_ReadPin(WK_UP_GPIO_Port, WK_UP_Pin)  	/* ��ȡWKUP���� */

#define KEY0_PRES    1              /* KEY0���� */
#define KEY1_PRES    2              /* KEY1���� */
#define WKUP_PRES    3              /* KEY_UP����(��WK_UP) */

uint8_t key_scan(uint8_t mode);     /* ����ɨ�躯�� */

#endif /* KEY_KEY_H_ */
