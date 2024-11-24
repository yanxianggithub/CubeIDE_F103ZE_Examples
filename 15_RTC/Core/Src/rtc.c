/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    rtc.c
  * @brief   This file provides code for the configuration
  *          of the RTC instances.
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
/* Includes ------------------------------------------------------------------*/
#include "rtc.h"

/* USER CODE BEGIN 0 */
_calendar_obj calendar;         /* ʱ�ӽṹ�� */

/* ��̬���� */
static uint8_t rtc_is_leap_year(uint16_t year);     /* �жϵ�ǰ����ǲ������� */
static long rtc_date2sec(uint16_t syear, uint8_t smon, uint8_t sday, uint8_t hour, uint8_t min, uint8_t sec);   /* ��������ʱ����ת���������� */


/* USER CODE END 0 */

RTC_HandleTypeDef hrtc;

/* RTC init function */
void MX_RTC_Init(void)
{

  /* USER CODE BEGIN RTC_Init 0 */
    /* ����ǲ��ǵ�һ������ʱ�� */
    uint16_t bkpflag = 0;
  /* USER CODE END RTC_Init 0 */

  RTC_TimeTypeDef sTime = {0};
  RTC_DateTypeDef DateToUpdate = {0};

  /* USER CODE BEGIN RTC_Init 1 */

  /* USER CODE END RTC_Init 1 */

  /** Initialize RTC Only
  */
  hrtc.Instance = RTC;
  hrtc.Init.AsynchPrediv = RTC_AUTO_1_SECOND;
  hrtc.Init.OutPut = RTC_OUTPUTSOURCE_NONE;
  if (HAL_RTC_Init(&hrtc) != HAL_OK)
  {
    Error_Handler();
  }

  /* USER CODE BEGIN Check_RTC_BKUP */
  bkpflag = rtc_read_bkr(0);  	/* ��ȡBKP0��ֵ */
  if (bkpflag != 0X5050)    	/* ֮ǰδ��ʼ�������������� */
  {
  /* USER CODE END Check_RTC_BKUP */

  /** Initialize RTC and set the Time and Date
  */
  sTime.Hours = 0x9;
  sTime.Minutes = 0x0;
  sTime.Seconds = 0x0;

  if (HAL_RTC_SetTime(&hrtc, &sTime, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  DateToUpdate.WeekDay = RTC_WEEKDAY_WEDNESDAY;
  DateToUpdate.Month = RTC_MONTH_AUGUST;
  DateToUpdate.Date = 0x23;
  DateToUpdate.Year = 0x23;

  if (HAL_RTC_SetDate(&hrtc, &DateToUpdate, RTC_FORMAT_BCD) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN RTC_Init 2 */
	  rtc_set_time(2023, 9, 10, 12, 0, 0);       	/* ����ʱ�� */
	  rtc_write_bkr(0, 0X5050);						/* д��0x5050���Ա㲻���ظ���ʼ��RTC��ʱ�� */
  }
  __HAL_RTC_ALARM_ENABLE_IT(&hrtc, RTC_IT_SEC);   	/* �������ж� */
  rtc_get_time(); /* ����ʱ�� */
  /* USER CODE END RTC_Init 2 */

}

void HAL_RTC_MspInit(RTC_HandleTypeDef* rtcHandle)
{

  if(rtcHandle->Instance==RTC)
  {
  /* USER CODE BEGIN RTC_MspInit 0 */

  /* USER CODE END RTC_MspInit 0 */
    HAL_PWR_EnableBkUpAccess();
    /* Enable BKP CLK enable for backup registers */
    __HAL_RCC_BKP_CLK_ENABLE();
    /* RTC clock enable */
    __HAL_RCC_RTC_ENABLE();

    /* RTC interrupt Init */
    HAL_NVIC_SetPriority(RTC_IRQn, 2, 2);
    HAL_NVIC_EnableIRQ(RTC_IRQn);
  /* USER CODE BEGIN RTC_MspInit 1 */

  /* USER CODE END RTC_MspInit 1 */
  }
}

void HAL_RTC_MspDeInit(RTC_HandleTypeDef* rtcHandle)
{

  if(rtcHandle->Instance==RTC)
  {
  /* USER CODE BEGIN RTC_MspDeInit 0 */

  /* USER CODE END RTC_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_RTC_DISABLE();

    /* RTC interrupt Deinit */
    HAL_NVIC_DisableIRQ(RTC_IRQn);
  /* USER CODE BEGIN RTC_MspDeInit 1 */

  /* USER CODE END RTC_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
/**
 * @brief       RTCд�������SRAM
 * @param       bkrx : �����Ĵ������,��Χ:0~41
                        ��Ӧ RTC_BKP_DR1~RTC_BKP_DR42
 * @param       data : Ҫд�������,16λ����
 * @retval      ��
 */
void rtc_write_bkr(uint32_t bkrx, uint16_t data)
{
    HAL_PWR_EnableBkUpAccess(); /* ȡ��������д���� */
    HAL_RTCEx_BKUPWrite(&hrtc, bkrx + 1, data);
}

/**
 * @brief       RTC��ȡ������SRAM
 * @param       bkrx : �����Ĵ������,��Χ:0~41
                ��Ӧ RTC_BKP_DR1~RTC_BKP_DR42
 * @retval      ��ȡ����ֵ
 */
uint16_t rtc_read_bkr(uint32_t bkrx)
{
    uint32_t temp = 0;
    temp = HAL_RTCEx_BKUPRead(&hrtc, bkrx + 1);
    return (uint16_t)temp; /* ���ض�ȡ����ֵ */
}

/**
 * @brief       �ж�����Ƿ�������
 *   @note      �·�������:
 *              �·�   1  2  3  4  5  6  7  8  9  10 11 12
 *              ����   31 29 31 30 31 30 31 31 30 31 30 31
 *              ������ 31 28 31 30 31 30 31 31 30 31 30 31
 * @param       year : ���
 * @retval      0, ������; 1, ������;
 */
static uint8_t rtc_is_leap_year(uint16_t year)
{
    /* �������: ��������겻���İ������� */
    if ((year % 4 == 0 && year % 100 != 0) || (year % 400 == 0))
    {
        return 1;
    }
    else
    {
        return 0;
    }
}

/**
 * @brief       ����ʱ��, ����������ʱ����
 *   @note      ��1970��1��1��Ϊ��׼, �����ۼ�ʱ��
 *              �Ϸ���ݷ�ΧΪ: 1970 ~ 2105��
                HALĬ��Ϊ������Ϊ2000��
 * @param       syear : ���
 * @param       smon  : �·�
 * @param       sday  : ����
 * @param       hour  : Сʱ
 * @param       min   : ����
 * @param       sec   : ����
 * @retval      0, �ɹ�; 1, ʧ��;
 */
uint8_t rtc_set_time(uint16_t syear, uint8_t smon, uint8_t sday, uint8_t hour, uint8_t min, uint8_t sec)
{
    uint32_t seccount = 0;

    seccount = rtc_date2sec(syear, smon, sday, hour, min, sec); /* ��������ʱ����ת������������ */

    __HAL_RCC_PWR_CLK_ENABLE(); /* ʹ�ܵ�Դʱ�� */
    __HAL_RCC_BKP_CLK_ENABLE(); /* ʹ�ܱ�����ʱ�� */
    HAL_PWR_EnableBkUpAccess(); /* ȡ��������д���� */
    /* ���������Ǳ����! */

    RTC->CRL |= 1 << 4;         /* �������� */

    RTC->CNTL = seccount & 0xffff;
    RTC->CNTH = seccount >> 16;

    RTC->CRL &= ~(1 << 4);      /* ���ø��� */

    while (!__HAL_RTC_ALARM_GET_FLAG(&hrtc, RTC_FLAG_RTOFF));       /* �ȴ�RTC�Ĵ����������, ���ȴ�RTOFF == 1 */

    return 0;
}

/**
 * @brief       ��������, ���嵽������ʱ����
 *   @note      ��1970��1��1��Ϊ��׼, �����ۼ�ʱ��
 *              �Ϸ���ݷ�ΧΪ: 1970 ~ 2105��
 * @param       syear : ���
 * @param       smon  : �·�
 * @param       sday  : ����
 * @param       hour  : Сʱ
 * @param       min   : ����
 * @param       sec   : ����
 * @retval      0, �ɹ�; 1, ʧ��;
 */
uint8_t rtc_set_alarm(uint16_t syear, uint8_t smon, uint8_t sday, uint8_t hour, uint8_t min, uint8_t sec)
{
    uint32_t seccount = 0;

    seccount = rtc_date2sec(syear, smon, sday, hour, min, sec); /* ��������ʱ����ת������������ */

    __HAL_RCC_PWR_CLK_ENABLE(); /* ʹ�ܵ�Դʱ�� */
    __HAL_RCC_BKP_CLK_ENABLE(); /* ʹ�ܱ�����ʱ�� */
    HAL_PWR_EnableBkUpAccess(); /* ȡ��������д���� */
    /* ���������Ǳ����! */

    RTC->CRL |= 1 << 4;         /* �������� */

    RTC->ALRL = seccount & 0xffff;
    RTC->ALRH = seccount >> 16;

    RTC->CRL &= ~(1 << 4);      /* ���ø��� */

    while (!__HAL_RTC_ALARM_GET_FLAG(&hrtc, RTC_FLAG_RTOFF));       /* �ȴ�RTC�Ĵ����������, ���ȴ�RTOFF == 1 */

    return 0;
}

/**
 * @brief       �õ���ǰ��ʱ��
 *   @note      �ú�����ֱ�ӷ���ʱ��, ʱ�����ݱ�����calendar�ṹ������
 * @param       ��
 * @retval      ��
 */
void rtc_get_time(void)
{
    static uint16_t daycnt = 0;
    uint32_t seccount = 0;
    uint32_t temp = 0;
    uint16_t temp1 = 0;
    const uint8_t month_table[12] = {31, 28, 31, 30, 31, 30, 31, 31, 30, 31, 30, 31}; /* ƽ����·����ڱ� */

    seccount = RTC->CNTH;       /* �õ��������е�ֵ(������) */
    seccount <<= 16;
    seccount += RTC->CNTL;

    temp = seccount / 86400;    /* �õ�����(��������Ӧ��) */

    if (daycnt != temp)         /* ����һ���� */
    {
        daycnt = temp;
        temp1 = 1970;           /* ��1970�꿪ʼ */

        while (temp >= 365)
        {
            if (rtc_is_leap_year(temp1)) /* ������ */
            {
                if (temp >= 366)
                {
                    temp -= 366; /* ����������� */
                }
                else
                {
                    break;
                }
            }
            else
            {
                temp -= 365;    /* ƽ�� */
            }

            temp1++;
        }

        calendar.year = temp1;  /* �õ���� */
        temp1 = 0;

        while (temp >= 28)      /* ������һ���� */
        {
            if (rtc_is_leap_year(calendar.year) && temp1 == 1) /* �����ǲ�������/2�·� */
            {
                if (temp >= 29)
                {
                    temp -= 29; /* ����������� */
                }
                else
                {
                    break;
                }
            }
            else
            {
                if (temp >= month_table[temp1])
                {
                    temp -= month_table[temp1]; /* ƽ�� */
                }
                else
                {
                    break;
                }
            }

            temp1++;
        }

        calendar.month = temp1 + 1; /* �õ��·� */
        calendar.date = temp + 1;   /* �õ����� */
    }

    temp = seccount % 86400;                                                    /* �õ������� */
    calendar.hour = temp / 3600;                                                /* Сʱ */
    calendar.min = (temp % 3600) / 60;                                          /* ���� */
    calendar.sec = (temp % 3600) % 60;                                          /* ���� */
    calendar.week = rtc_get_week(calendar.year, calendar.month, calendar.date); /* ��ȡ���� */
}

/**
 * @brief       ��������ʱ����ת����������
 *   @note      ���빫�����ڵõ�����(��ʼʱ��Ϊ: ��Ԫ0��3��1�տ�ʼ, ����������κ�����, �����Ի�ȡ��ȷ������)
 *              ʹ�� ��ķ����ɭ���㹫ʽ ����, ԭ��˵��������:
 *              https://www.cnblogs.com/fengbohello/p/3264300.html
 * @param       syear : ���
 * @param       smon  : �·�
 * @param       sday  : ����
 * @retval      0, ������; 1 ~ 6: ����һ ~ ������
 */
uint8_t rtc_get_week(uint16_t year, uint8_t month, uint8_t day)
{
    uint8_t week = 0;

    if (month < 3)
    {
        month += 12;
        --year;
    }

    week = (day + 1 + 2 * month + 3 * (month + 1) / 5 + year + (year >> 2) - year / 100 + year / 400) % 7;
    return week;
}

/**
 * @brief       ��������ʱ����ת����������
 *   @note      ��1970��1��1��Ϊ��׼, 1970��1��1��, 0ʱ0��0��, ��ʾ��0����
 *              ����ʾ��2105��, ��Ϊuint32_t����ʾ136���������(����������)!
 *              ������ο�ֻlinux mktime����, ԭ��˵��������:
 *              http://www.openedv.com/thread-63389-1-1.html
 * @param       syear : ���
 * @param       smon  : �·�
 * @param       sday  : ����
 * @param       hour  : Сʱ
 * @param       min   : ����
 * @param       sec   : ����
 * @retval      ת�����������
 */
static long rtc_date2sec(uint16_t syear, uint8_t smon, uint8_t sday, uint8_t hour, uint8_t min, uint8_t sec)
{
    uint32_t Y, M, D, X, T;
    signed char monx = smon;    /* ���·�ת���ɴ����ŵ�ֵ, ����������� */

    if (0 >= (monx -= 2))       /* 1..12 -> 11,12,1..10 */
    {
        monx += 12;             /* Puts Feb last since it has leap day */
        syear -= 1;
    }

    Y = (syear - 1) * 365 + syear / 4 - syear / 100 + syear / 400; /* ��ԪԪ��1�����ڵ������� */
    M = 367 * monx / 12 - 30 + 59;
    D = sday - 1;
    X = Y + M + D - 719162;                      /* ��ȥ��ԪԪ�굽1970������� */
    T = ((X * 24 + hour) * 60 + min) * 60 + sec; /* �������� */
    return T;
}

/**
 * @brief       �õ���ǰ��ʱ��
 * @note      	�ú�����ֱ�ӷ���ʱ��, ʱ�����ݱ�����calendar�ṹ������
 * @param       ��
 * @retval      ��
 */
void hal_rtc_get_tim(void)
{
  RTC_DateTypeDef sdatestructureget;
  RTC_TimeTypeDef stimestructureget;

  /* Get the RTC current Time */
  HAL_RTC_GetTime(&hrtc, &stimestructureget, RTC_FORMAT_BIN);
  /* Get the RTC current Date */
  HAL_RTC_GetDate(&hrtc, &sdatestructureget, RTC_FORMAT_BIN);
  calendar.hour = stimestructureget.Hours;                                   	/* Сʱ */
  calendar.min = stimestructureget.Minutes;                                 	/* ���� */
  calendar.sec = stimestructureget.Seconds;                                 	/* ���� */
  calendar.year = 2000 + sdatestructureget.Year;								/* �� */
  calendar.month = sdatestructureget.Month;										/* �� */
  calendar.date = sdatestructureget.Date;										/* �� */
  calendar.week = rtc_get_week(calendar.year, calendar.month, calendar.date);	/* ���ں� */
}
/* USER CODE END 1 */
