/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    tim.c
  * @brief   This file provides code for the configuration
  *          of the TIM instances.
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
#include "tim.h"

/* USER CODE BEGIN 0 */
#include "../../BSP/LED/led.h"
/* USER CODE END 0 */

TIM_HandleTypeDef htim5;

/* TIM5 init function */
void MX_TIM5_Init(void)
{

  /* USER CODE BEGIN TIM5_Init 0 */

  /* USER CODE END TIM5_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_IC_InitTypeDef sConfigIC = {0};

  /* USER CODE BEGIN TIM5_Init 1 */

  /* USER CODE END TIM5_Init 1 */
  htim5.Instance = TIM5;
  htim5.Init.Prescaler = 72 - 1;
  htim5.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim5.Init.Period = 65535;
  htim5.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim5.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim5) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim5, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_IC_Init(&htim5) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim5, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 0;
  if (HAL_TIM_IC_ConfigChannel(&htim5, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM5_Init 2 */
  __HAL_TIM_ENABLE_IT(&htim5, TIM_IT_UPDATE);     	/* ʹ�ܸ����ж� */
  HAL_TIM_IC_Start_IT(&htim5, TIM_CHANNEL_1);    	/* ��ʼ����TIM5��ͨ��1 */
  /* USER CODE END TIM5_Init 2 */

}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* tim_baseHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(tim_baseHandle->Instance==TIM5)
  {
  /* USER CODE BEGIN TIM5_MspInit 0 */

  /* USER CODE END TIM5_MspInit 0 */
    /* TIM5 clock enable */
    __HAL_RCC_TIM5_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**TIM5 GPIO Configuration
    PA0-WKUP     ------> TIM5_CH1
    */
    GPIO_InitStruct.Pin = WK_UP_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(WK_UP_GPIO_Port, &GPIO_InitStruct);

    /* TIM5 interrupt Init */
    HAL_NVIC_SetPriority(TIM5_IRQn, 2, 1);
    HAL_NVIC_EnableIRQ(TIM5_IRQn);
  /* USER CODE BEGIN TIM5_MspInit 1 */

  /* USER CODE END TIM5_MspInit 1 */
  }
}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* tim_baseHandle)
{

  if(tim_baseHandle->Instance==TIM5)
  {
  /* USER CODE BEGIN TIM5_MspDeInit 0 */

  /* USER CODE END TIM5_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM5_CLK_DISABLE();

    /**TIM5 GPIO Configuration
    PA0-WKUP     ------> TIM5_CH1
    */
    HAL_GPIO_DeInit(WK_UP_GPIO_Port, WK_UP_Pin);

    /* TIM5 interrupt Deinit */
    HAL_NVIC_DisableIRQ(TIM5_IRQn);
  /* USER CODE BEGIN TIM5_MspDeInit 1 */

  /* USER CODE END TIM5_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */

/* ���벶��״̬(g_timxchy_cap_sta)
 * [7]  :0,û�гɹ��Ĳ���;1,�ɹ�����һ��.
 * [6]  :0,��û���񵽸ߵ�ƽ;1,�Ѿ����񵽸ߵ�ƽ��.
 * [5:0]:����ߵ�ƽ������Ĵ���,������63��,���������ֵ = 63*65536 + 65535 = 4194303
 *       ע��:Ϊ��ͨ��,����Ĭ��ARR��CCRy����16λ�Ĵ���,����32λ�Ķ�ʱ��(��:TIM5),Ҳֻ��16λʹ��
 *       ��1us�ļ���Ƶ��,����ʱ��Ϊ:4194303 us, Լ4.19��
 *
 *      (˵��һ�£�����32λ��ʱ����˵,1us��������1,���ʱ��:4294��)
 */
uint8_t g_timxchy_cap_sta = 0;    /* ���벶��״̬ */
uint16_t g_timxchy_cap_val = 0;   /* ���벶��ֵ */

/**
 * @brief       ��ʱ�����벶���жϴ���ص�����
 * @param       htim:��ʱ�����ָ��
 * @note        �ú�����HAL_TIM_IRQHandler�лᱻ����
 * @retval      ��
 */
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM5)
    {
        if ((g_timxchy_cap_sta & 0X80) == 0)                	/* ��δ�ɹ����� */
        {
            if (g_timxchy_cap_sta & 0X40)                   	/* ����һ���½��� */
            {
                g_timxchy_cap_sta |= 0X80;                  	/* ��ǳɹ�����һ�θߵ�ƽ���� */
                g_timxchy_cap_val = HAL_TIM_ReadCapturedValue(htim, TIM_CHANNEL_1);  /* ��ȡ��ǰ�Ĳ���ֵ */
                __HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING);/* ����TIM5ͨ��1�����ز��� */
            }
            else /* ��δ��ʼ,��һ�β��������� */
            {
                g_timxchy_cap_sta = 0;                      	/* ��� */
                g_timxchy_cap_val = 0;
                g_timxchy_cap_sta |= 0X40;                     	/* ��ǲ����������� */
                __HAL_TIM_DISABLE(htim);          				/* �رն�ʱ��5 */
                __HAL_TIM_SET_COUNTER(htim, 0);   				/* ��ʱ��5���������� */
                __HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_FALLING);/* ��ʱ��5ͨ��1����Ϊ�½��ز��� */
                __HAL_TIM_ENABLE(htim);           				/* ʹ�ܶ�ʱ��5 */
            }
        }
    }
}

/**
 * @brief       ��ʱ�������жϻص�����
 * @param        htim:��ʱ�����ָ��
 * @note        �˺����ᱻ��ʱ���жϺ�����ͬ���õ�
 * @retval      ��
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == TIM5)
    {
        if ((g_timxchy_cap_sta & 0X80) == 0)            /* ��δ�ɹ����� */
        {
            if (g_timxchy_cap_sta & 0X40)               /* �Ѿ����񵽸ߵ�ƽ�� */
            {
                if ((g_timxchy_cap_sta & 0X3F) == 0X3F) /* �ߵ�ƽ̫���� */
                {
                	__HAL_TIM_SET_CAPTUREPOLARITY(htim, TIM_CHANNEL_1, TIM_INPUTCHANNELPOLARITY_RISING);/* ����TIM5ͨ��1�����ز��� */
                    g_timxchy_cap_sta |= 0X80;          /* ��ǳɹ�������һ�� */
                    g_timxchy_cap_val = 0XFFFF;
                }
                else      /* �ۼƶ�ʱ��������� */
                {
                    g_timxchy_cap_sta++;
                }
            }
        }
    }
}
/* USER CODE END 1 */
