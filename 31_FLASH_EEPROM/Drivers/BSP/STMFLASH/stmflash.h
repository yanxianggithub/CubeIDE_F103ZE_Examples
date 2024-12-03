
#ifndef __STMFLASH_H
#define __STMFLASH_H

#include "../../SYSTEM/sys/sys.h"


/* FLASH��ʼ��ַ */
#define STM32_FLASH_SIZE        0x80000         /* STM32 FLASH �ܴ�С */
#define STM32_FLASH_BASE        0x08000000      /* STM32 FLASH ��ʼ��ַ */

 /* STM32F103 ������С */
#if STM32_FLASH_SIZE < 256 * 1024
#define STM32_SECTOR_SIZE   1024                /* ����С��256K�� F103, ������СΪ1K�ֽ� */
#else
#define STM32_SECTOR_SIZE   2048                /* �������ڵ�����256K�� F103, ������СΪ2K�ֽ� */
#endif

/* FLASH������ֵ */
#define STM32_FLASH_KEY1        0X45670123
#define STM32_FLASH_KEY2        0XCDEF89AB

/* �ӿں���(�ⲿ�ɵ���) */
uint16_t stmflash_read_halfword(uint32_t faddr);                        /* FLASH������ */
void stmflash_read(uint32_t raddr, uint16_t *pbuf, uint16_t length);    /* ��ָ����ַ��ʼ����ָ�����ȵ����� */
void stmflash_write(uint32_t waddr, uint16_t *pbuf, uint16_t length);   /* ��FLASH ָ��λ��, д��ָ�����ȵ�����(�Զ�����) */

/* ���Ժ��� */
void test_write(uint32_t waddr, uint16_t wdata);

#endif


















