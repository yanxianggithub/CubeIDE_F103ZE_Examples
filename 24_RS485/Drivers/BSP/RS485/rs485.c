
#include "rs485.h"
#include "../../SYSTEM/delay/delay.h"
#include "usart.h"


uint8_t g_RS485_rx_buf[RS485_REC_LEN]; /* 接收缓冲, 最大 RS485_REC_LEN 个字节. */
uint8_t g_RS485_rx_cnt = 0;            /* 接收到的数据长度 */

/**
 * @brief       RS485初始化函数
 *   @note      该函数主要是初始化串口
 * @param       baudrate: 波特率, 根据自己需要设置波特率值
 * @retval      无
 */
void rs485_init(uint32_t baudrate)
{
	__HAL_UART_ENABLE_IT(&huart2, UART_IT_RXNE); 	/* 开启接收中断 */
    RS485_RE(0); 									/* 默认为接收模式 */
}

/**
 * @brief       RS485发送len个字节
 * @param       buf     : 发送区首地址
 * @param       len     : 发送的字节数(为了和本代码的接收匹配,这里建议不要超过 RS485_REC_LEN 个字节)
 * @retval      无
 */
void rs485_send_data(uint8_t *buf, uint8_t len)
{
    RS485_RE(1);                             	/* 进入发送模式 */
    HAL_UART_Transmit(&huart2, buf, len, 1000); /* 串口2发送数据 */
    g_RS485_rx_cnt = 0;
    RS485_RE(0); /* 进入接收模式 */
}

/**
 * @brief       RS485查询接收到的数据
 * @param       buf     : 接收缓冲区首地址
 * @param       len     : 接收到的数据长度
 *   @arg               0   , 表示没有接收到任何数据
 *   @arg               其他, 表示接收到的数据长度
 * @retval      无
 */
void rs485_receive_data(uint8_t *buf, uint8_t *len)
{
    uint8_t rxlen = g_RS485_rx_cnt;
    uint8_t i = 0;
    *len = 0;     /* 默认为0 */
    delay_ms(10); /* 等待10ms,连续超过10ms没有接收到一个数据,则认为接收结束 */

    if (rxlen == g_RS485_rx_cnt && rxlen) /* 接收到了数据,且接收完成了 */
    {
        for (i = 0; i < rxlen; i++)
        {
            buf[i] = g_RS485_rx_buf[i];
        }

        *len = g_RS485_rx_cnt; /* 记录本次数据长度 */
        g_RS485_rx_cnt = 0;    /* 清零 */
    }
}
