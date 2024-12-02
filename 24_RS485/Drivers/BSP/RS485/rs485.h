
#ifndef __RS485_H
#define __RS485_H

#include "../../SYSTEM/sys/sys.h"

/******************************************************************************************/

/* 控制RS485_RE脚, 控制RS485发送/接收状态
 * RS485_RE = 0, 进入接收模式
 * RS485_RE = 1, 进入发送模式
 */
#define RS485_RE(x)   do{ x ? \
                          HAL_GPIO_WritePin(RS485_RE_GPIO_Port, RS485_RE_Pin, GPIO_PIN_SET) : \
                          HAL_GPIO_WritePin(RS485_RE_GPIO_Port, RS485_RE_Pin, GPIO_PIN_RESET); \
                      }while(0)


#define RS485_REC_LEN               64          /* 定义最大接收字节数 64 */

extern uint8_t g_RS485_rx_buf[RS485_REC_LEN];   /* 接收缓冲,最大RS485_REC_LEN个字节 */
extern uint8_t g_RS485_rx_cnt;                  /* 接收数据长度 */


void rs485_init( uint32_t baudrate);  /* RS485初始化 */
void rs485_send_data(uint8_t *buf, uint8_t len);    /* RS485发送数据 */
void rs485_receive_data(uint8_t *buf, uint8_t *len);/* RS485接收数据 */

#endif
















