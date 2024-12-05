/**
 * V3.4 20200324
 * 1, 新增usmart_port.c和usmart_port.h,用于管理USMART的移植,方便修改
 * 2, 修改变量命名方式为: uint8_t, uint16_t, uint32_t
 * 3, 修改usmart_reset_runtime为usmart_timx_reset_time
 * 4, 修改usmart_get_runtime为usmart_timx_get_time
 * 5, 修改usmart_scan函数实现方式,改成由usmart_get_input_string获取数据流
 * 6, 修改printf函数为USMART_PRINTF宏定义
 * 7, 修改定时扫描相关函数,改用宏定义方式,方便移植
 * V3.5 20201220
 * 1，修改部分代码以支持AC6编译器
 *
 ****************************************************************************************************
 */

#ifndef __USMART_STR_H
#define __USMART_STR_H

#include "usmart_port.h"


uint8_t usmart_get_parmpos(uint8_t num);                /* 得到某个参数在参数列里面的起始位置 */
uint8_t usmart_strcmp(char *str1, char *str2);    /* 对比两个字符串是否相等 */
uint32_t usmart_pow(uint8_t m, uint8_t n);              /* M^N次方 */
uint8_t usmart_str2num(char *str, uint32_t *res);    /* 字符串转为数字 */
uint8_t usmart_get_cmdname(char *str, char *cmdname, uint8_t *nlen, uint8_t maxlen); /* 从str中得到指令名,并返回指令长度 */
uint8_t usmart_get_fname(char *str, char *fname, uint8_t *pnum, uint8_t *rval); /* 从str中得到函数名 */
uint8_t usmart_get_aparm(char *str, char *fparm, uint8_t *ptype); /* 从str中得到一个函数参数 */
uint8_t usmart_get_fparam(char *str, uint8_t *parn); /* 得到str中所有的函数参数. */

#endif











