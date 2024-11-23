
#ifndef KEY_KEY_H_
#define KEY_KEY_H_
#include "../../SYSTEM/sys/sys.h"

#define KEY0        HAL_GPIO_ReadPin(KEY0_GPIO_Port, KEY0_Pin)     	/* 读取KEY0引脚 */
#define KEY1        HAL_GPIO_ReadPin(KEY1_GPIO_Port, KEY1_Pin)     	/* 读取KEY1引脚 */
#define WK_UP       HAL_GPIO_ReadPin(WK_UP_GPIO_Port, WK_UP_Pin)  	/* 读取WKUP引脚 */

#define KEY0_PRES    1              /* KEY0按下 */
#define KEY1_PRES    2              /* KEY1按下 */
#define WKUP_PRES    3              /* KEY_UP按下(即WK_UP) */

uint8_t key_scan(uint8_t mode);     /* 按键扫描函数 */

#endif /* KEY_KEY_H_ */
