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

/* ʱ��ṹ��, ������������ʱ�������Ϣ */
typedef struct
{
    uint8_t hour;       /* ʱ */
    uint8_t min;        /* �� */
    uint8_t sec;        /* �� */
    /* ������������ */
    uint16_t year;      /* �� */
    uint8_t  month;     /* �� */
    uint8_t  date;      /* �� */
    uint8_t  week;      /* �� */
} _calendar_obj;
extern _calendar_obj calendar;                      /* ʱ��ṹ�� */
/* USER CODE END Private defines */

void MX_RTC_Init(void);

/* USER CODE BEGIN Prototypes */

/* �ӿں��� */
void rtc_get_time(void);                            /* ��ȡRTCʱ����Ϣ */
void hal_rtc_get_tim(void);							/* �ٷ���ȡRTCʱ����Ϣ */
uint16_t rtc_read_bkr(uint32_t bkrx);               /* ��ȡ�󱸼Ĵ��� */
void rtc_write_bkr(uint32_t bkrx, uint16_t data);   /* д�󱸼Ĵ��� */
uint8_t rtc_get_week(uint16_t year, uint8_t month, uint8_t day);    /* ���������ջ�ȡ���ڼ� */
uint8_t rtc_set_time(uint16_t syear, uint8_t smon, uint8_t sday, uint8_t hour, uint8_t min, uint8_t sec);   /* ����ʱ�� */
uint8_t rtc_set_alarm(uint16_t syear, uint8_t smon, uint8_t sday, uint8_t hour, uint8_t min, uint8_t sec);  /* ��������ʱ�� */

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __RTC_H__ */

