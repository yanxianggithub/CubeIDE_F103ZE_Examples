/**
 ****************************************************************************************************
 * @file        atk_ncr.h
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2020-04-04
 * @brief       ������ĸʶ�� ����
 *   @note      ������ĸʶ�����������ԭ��(ALIENTEK)�ṩ, �����ṩ2��LIB, �����ʹ��
 *              ATKNCR_M_Vx.x.lib��ATKNCR_N_Vx.x.lib��Ψһ�������Ƿ�ʹ�ö�̬�ڴ����
 *              ����:
 *              M,������Ҫ�õ�malloc�İ汾,����ʵ��alientek_ncr_malloc��alientek_ncr_free��������
 *              N,������ͨ�汾,����Ҫʵ��alientek_ncr_malloc��alientek_ncr_free��������
 *              Vx.x,������ǰʶ�����İ汾
 *
 *              ����: ֧������/Сд��ĸ/��д��ĸ/�������ʶ��ģʽ
 *              ��ʶ�����ʹ�������൱��, ��������:
 *              ��һ��:����alientek_ncr_init����,��ʼ��ʶ�����
 *              �ڶ���:��ȡ����ĵ�������(������2�������ϵĲ�ͬ������������)
 *              ������:����alientek_ncr����,�õ�ʶ����
 *              ���Ĳ�:�������Ҫ��ʶ��,�����alientek_ncr_stop����,��ֹʶ��.�������Ҫ����,���ظ�2,3������
 *
 * @license     Copyright (c) 2020-2032, �������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:����ԭ�� STM32������
 * ������Ƶ:www.yuanzige.com
 * ������̳:www.openedv.com
 * ��˾��ַ:www.alientek.com
 * �����ַ:openedv.taobao.com
 *
 * �޸�˵��
 * V1.0 20200404
 * ��һ�η���
 *
 ****************************************************************************************************
 */

#ifndef __ATK_NCR_H
#define __ATK_NCR_H

/* ��ʹ��ATKNCR_M_Vx.x.lib��ʱ��,����Ҫ����ATK_NCR_TRACEBUF1_SIZE��ATK_NCR_TRACEBUF2_SIZE
 * ��ʹ��ATKNCR_N_Vx.x.lib��ʱ��,�������ʶ������,���ʵ�����ATK_NCR_TRACEBUF1_SIZE��ATK_NCR_TRACEBUF2_SIZE��ֵ
 */
#define ATK_NCR_TRACEBUF1_SIZE      500*4           /* �����һ��tracebuf��С(��λΪ�ֽ�),�����������,��Ѹ������ʵ��Ĵ� */
#define ATK_NCR_TRACEBUF2_SIZE      250*4           /* ����ڶ���tracebuf��С(��λΪ�ֽ�),�����������,��Ѹ������ʵ��Ĵ� */


/* ����켣�������� */
typedef __PACKED_STRUCT _atk_ncr_point
{
    short x;        /* x������ */
    short y;        /* y������ */
}atk_ncr_point;



/* �ⲿ���ú���
 * ��ʼ��ʶ����
 * ����ֵ : 0, ��ʼ���ɹ�
 *          1, ��ʼ��ʧ��
 */
unsigned char alientek_ncr_init(void);


/* ֹͣʶ���� */
void alientek_ncr_stop(void);

/* ʶ����ʶ��
 * track   : ������󼯺� 
 * potnum  : �������ĵ���,����track�Ĵ�С
 * charnum : ��������Ľ����,������ϣ��������ٸ�ƥ����
 * mode    : ʶ��ģʽ
 *          1,��ʶ������
 *          2,��ʶ���д��ĸ
 *          3,��ʶ��Сд��ĸ
 *          4,���ʶ��(ȫ��ʶ��)
 *
 * result  : ���������(����Ϊ:charnum+1���ֽ�)
 */
void alientek_ncr(atk_ncr_point * track,int potnum,int charnum,unsigned char mode,char*result);

/* �ڴ����ú��� */
void alientek_ncr_memset(char *p,char c,unsigned long len);

/* ��̬�����ڴ�,��ʹ��ATKNCR_M_Vx.x.libʱ,����ʵ�� */
void *alientek_ncr_malloc(unsigned int size);

/* ��̬�ͷ��ڴ�,��ʹ��ATKNCR_M_Vx.x.libʱ,����ʵ�� */
void alientek_ncr_free(void *ptr);

#endif


















