
#include "rs485.h"
#include "../../SYSTEM/delay/delay.h"
#include "usart.h"


uint8_t g_RS485_rx_buf[RS485_REC_LEN]; /* ���ջ���, ��� RS485_REC_LEN ���ֽ�. */
uint8_t g_RS485_rx_cnt = 0;            /* ���յ������ݳ��� */

/**
 * @brief       RS485��ʼ������
 *   @note      �ú�����Ҫ�ǳ�ʼ������
 * @param       baudrate: ������, �����Լ���Ҫ���ò�����ֵ
 * @retval      ��
 */
void rs485_init(uint32_t baudrate)
{
	__HAL_UART_ENABLE_IT(&huart2, UART_IT_RXNE); 	/* ���������ж� */
    RS485_RE(0); 									/* Ĭ��Ϊ����ģʽ */
}

/**
 * @brief       RS485����len���ֽ�
 * @param       buf     : �������׵�ַ
 * @param       len     : ���͵��ֽ���(Ϊ�˺ͱ�����Ľ���ƥ��,���ｨ�鲻Ҫ���� RS485_REC_LEN ���ֽ�)
 * @retval      ��
 */
void rs485_send_data(uint8_t *buf, uint8_t len)
{
    RS485_RE(1);                             	/* ���뷢��ģʽ */
    HAL_UART_Transmit(&huart2, buf, len, 1000); /* ����2�������� */
    g_RS485_rx_cnt = 0;
    RS485_RE(0); /* �������ģʽ */
}

/**
 * @brief       RS485��ѯ���յ�������
 * @param       buf     : ���ջ������׵�ַ
 * @param       len     : ���յ������ݳ���
 *   @arg               0   , ��ʾû�н��յ��κ�����
 *   @arg               ����, ��ʾ���յ������ݳ���
 * @retval      ��
 */
void rs485_receive_data(uint8_t *buf, uint8_t *len)
{
    uint8_t rxlen = g_RS485_rx_cnt;
    uint8_t i = 0;
    *len = 0;     /* Ĭ��Ϊ0 */
    delay_ms(10); /* �ȴ�10ms,��������10msû�н��յ�һ������,����Ϊ���ս��� */

    if (rxlen == g_RS485_rx_cnt && rxlen) /* ���յ�������,�ҽ�������� */
    {
        for (i = 0; i < rxlen; i++)
        {
            buf[i] = g_RS485_rx_buf[i];
        }

        *len = g_RS485_rx_cnt; /* ��¼�������ݳ��� */
        g_RS485_rx_cnt = 0;    /* ���� */
    }
}
