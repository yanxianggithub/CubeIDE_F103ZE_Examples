/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    dac.h
  * @brief   This file contains all the function prototypes for
  *          the dac.c file
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
#ifndef __DAC_H__
#define __DAC_H__

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "main.h"

/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

extern DAC_HandleTypeDef hdac;

/* USER CODE BEGIN Private defines */
extern DMA_HandleTypeDef hdma_dac_ch1;
/* USER CODE END Private defines */

void MX_DAC_Init(void);

/* USER CODE BEGIN Prototypes */
void dac_triangular_wave(uint16_t maxval, uint16_t dt, uint16_t samples, uint16_t n);
void dac_creat_sin_buf(uint16_t maxval, uint16_t samples);
void dac_dma_sin_set(uint16_t arr, uint16_t psc);
void dac_dma_wave_enable(uint16_t cndtr, uint16_t arr, uint16_t psc);
/* USER CODE END Prototypes */

#ifdef __cplusplus
}
#endif

#endif /* __DAC_H__ */

