
#ifndef __DS18B20_H
#define __DS18B20_H

#include "../../SYSTEM/sys/sys.h"

/******************************************************************************************/

/* IO�������� */
#define DS18B20_DQ_OUT(x)   do{ x ? \
                                HAL_GPIO_WritePin(DS18B20_DQ_GPIO_Port, DS18B20_DQ_Pin, GPIO_PIN_SET) : \
                                HAL_GPIO_WritePin(DS18B20_DQ_GPIO_Port, DS18B20_DQ_Pin, GPIO_PIN_RESET); \
                            }while(0)                                                       /* ���ݶ˿���� */
#define DS18B20_DQ_IN       HAL_GPIO_ReadPin(DS18B20_DQ_GPIO_Port, DS18B20_DQ_Pin)     /* ���ݶ˿����� */


uint8_t ds18b20_init(void);         /* ��ʼ��DS18B20 */
uint8_t ds18b20_check(void);        /* ����Ƿ����DS18B20 */
short ds18b20_get_temperature(void);/* ��ȡ�¶� */

#endif















