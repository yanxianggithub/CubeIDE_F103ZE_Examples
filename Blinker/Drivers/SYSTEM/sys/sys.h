/*
 * sys.h
 *
 *  Created on: Nov 18, 2024
 *      Author: yan
 */

#ifndef SYSTEM_SYS_SYS_H_
#define SYSTEM_SYS_SYS_H_

#include "stm32f1xx.h"
#include "main.h"

/**
 * SYS_SUPPORT_OS用于定义系统文件夹是否支持OS
 * 0,不支持OS
 * 1,支持OS
 */
#define SYS_SUPPORT_OS          0


/*函数申明*******************************************************************************************/

void sys_nvic_set_vector_table(uint32_t baseaddr, uint32_t offset);             /* 设置中断偏移量 */
void sys_standby(void);                                                         /* 进入待机模式 */
void sys_soft_reset(void);                                                      /* 系统软复位 */
uint8_t sys_clock_set(uint32_t plln);                                           /* 时钟设置函数 */
void sys_stm32_clock_init(uint32_t plln);                                       /* 系统时钟初始化函数 */

/* 以下为汇编函数 */
void sys_wfi_set(void);                                                         /* 执行WFI指令 */
void sys_intx_disable(void);                                                    /* 关闭所有中断 */
void sys_intx_enable(void);                                                     /* 开启所有中断 */
void sys_msr_msp(uint32_t addr);                                                /* 设置栈顶地址 */

#endif /* SYSTEM_SYS_SYS_H_ */
