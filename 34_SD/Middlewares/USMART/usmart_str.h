/**
 * V3.4 20200324
 * 1, ����usmart_port.c��usmart_port.h,���ڹ���USMART����ֲ,�����޸�
 * 2, �޸ı���������ʽΪ: uint8_t, uint16_t, uint32_t
 * 3, �޸�usmart_reset_runtimeΪusmart_timx_reset_time
 * 4, �޸�usmart_get_runtimeΪusmart_timx_get_time
 * 5, �޸�usmart_scan����ʵ�ַ�ʽ,�ĳ���usmart_get_input_string��ȡ������
 * 6, �޸�printf����ΪUSMART_PRINTF�궨��
 * 7, �޸Ķ�ʱɨ����غ���,���ú궨�巽ʽ,������ֲ
 * V3.5 20201220
 * 1���޸Ĳ��ִ�����֧��AC6������
 *
 ****************************************************************************************************
 */

#ifndef __USMART_STR_H
#define __USMART_STR_H

#include "usmart_port.h"


uint8_t usmart_get_parmpos(uint8_t num);                /* �õ�ĳ�������ڲ������������ʼλ�� */
uint8_t usmart_strcmp(char *str1, char *str2);    /* �Ա������ַ����Ƿ���� */
uint32_t usmart_pow(uint8_t m, uint8_t n);              /* M^N�η� */
uint8_t usmart_str2num(char *str, uint32_t *res);    /* �ַ���תΪ���� */
uint8_t usmart_get_cmdname(char *str, char *cmdname, uint8_t *nlen, uint8_t maxlen); /* ��str�еõ�ָ����,������ָ��� */
uint8_t usmart_get_fname(char *str, char *fname, uint8_t *pnum, uint8_t *rval); /* ��str�еõ������� */
uint8_t usmart_get_aparm(char *str, char *fparm, uint8_t *ptype); /* ��str�еõ�һ���������� */
uint8_t usmart_get_fparam(char *str, uint8_t *parn); /* �õ�str�����еĺ�������. */

#endif











