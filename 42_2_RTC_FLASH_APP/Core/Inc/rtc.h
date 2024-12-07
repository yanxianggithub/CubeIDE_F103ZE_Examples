/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    rtc.h
  * @brief   This file contains all the function prototypes for
  *          the rtc.c file
  ******************************************************************************
  * @attention
  *
  * Copyright (c) 2023 STMicroelectronics.
  * All rights reserved.
  *
  * This software is licensed under terms that can be found in the LICENSE file
  * in the root directory of this software component.
  * If no LICENSE file comes with this software, it is provided AS-IS.
  *
  ******************************************************************************
  */
/* USER CODE END Header */
/* Define to prevent recursive inclusion -------------------------------------*/
#ifndef __RTC_H__
#define __RTC_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern RTC_HandleTypeDef hrtc;

/* USER CODE BEGIN Private defines */

/* 时间结构体, 包括年月日周时分秒等信息 */
typedef struct
{
    uint8_t hour;       /* 时 */
    uint8_t min;        /* 分 */
    uint8_t sec;        /* 秒 */
    /* 公历年月日周 */
    uint16_t year;      /* 年 */
    uint8_t  month;     /* 月 */
    uint8_t  date;      /* 日 */
    uint8_t  week;      /* 周 */
} _calendar_obj;
extern _calendar_obj calendar;                      /* 时间结构体 */
/* USER CODE END Private defines */

void MX_RTC_Init(void);

/* USER CODE BEGIN Prototypes */

/* 接口函数 */
void rtc_get_time(void);                            /* 获取RTC时间信息 */
void hal_rtc_get_tim(void);							/* 官方获取RTC时间信息 */
uint16_t rtc_read_bkr(uint32_t bkrx);               /* 读取后备寄存器 */
void rtc_write_bkr(uint32_t bkrx, uint16_t data);   /* 写后备寄存器 */
uint8_t rtc_get_week(uint16_t year, uint8_t month, uint8_t day);    /* 根据年月日获取星期几 */
uint8_t rtc_set_time(uint16_t syear, uint8_t smon, uint8_t sday, uint8_t hour, uint8_t min, uint8_t sec);   /* 设置时间 */
uint8_t rtc_set_alarm(uint16_t syear, uint8_t smon, uint8_t sday, uint8_t hour, uint8_t min, uint8_t sec);  /* 设置闹钟时间 */

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __RTC_H__ */

