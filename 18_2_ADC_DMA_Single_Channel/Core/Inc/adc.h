/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    adc.h
  * @brief   This file contains all the function prototypes for
  *          the adc.c file
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
#ifndef __ADC_H__
#define __ADC_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern ADC_HandleTypeDef hadc1;

/* USER CODE BEGIN Private defines */
#define ADC_ADCX_DMACx_IS_TC()              ( DMA1->ISR & (1 << 1) )    /* �ж� DMA1_Channel1 ������ɱ�־, ����һ���ٺ�����ʽ,
                                                                         * ���ܵ�����ʹ��, ֻ������if���������                                                                    */
#define ADC_ADCX_DMACx_CLR_TC()             do{ DMA1->IFCR |= 1 << 1; }while(0) /* ��� DMA1_Channel1 ������ɱ�־ */

/* USER CODE END Private defines */

void MX_ADC1_Init(void);

/* USER CODE BEGIN Prototypes */
extern DMA_HandleTypeDef hdma_adc1;
void adc_channel_set(ADC_HandleTypeDef *adc_handle, uint32_t ch,uint32_t rank, uint32_t stime); /* ADCͨ������ */
uint32_t adc_get_result(uint32_t ch);                               /* ���ĳ��ͨ��ֵ  */
uint32_t adc_get_result_average(uint32_t ch, uint8_t times);        /* �õ�ĳ��ͨ����������������ƽ��ֵ */
void adc_dma_enable( uint16_t cndtr);                               /* ʹ��һ��ADC DMA�ɼ����� */

/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __ADC_H__ */

