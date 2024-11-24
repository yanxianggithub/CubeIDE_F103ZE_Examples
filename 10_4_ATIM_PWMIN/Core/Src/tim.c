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
#include "../../SYSTEM/sys/sys.h"
/* PWM输入状态(g_timxchy_cap_sta)
 * 0,没有成功捕获.
 * 1,已经成功捕获了
 */
uint8_t g_timxchy_pwmin_sta  = 0;   /* PWM输入状态 */
uint16_t g_timxchy_pwmin_psc  = 0;  /* PWM输入分频系数 */
uint32_t g_timxchy_pwmin_hval = 0;  /* PWM的高电平脉宽 */
uint32_t g_timxchy_pwmin_cval = 0;  /* PWM的周期宽度 */

/* USER CODE END 0 */

TIM_HandleTypeDef htim3;
TIM_HandleTypeDef htim8;

/* TIM3 init function */
void MX_TIM3_Init(void)
{

  /* USER CODE BEGIN TIM3_Init 0 */

  /* USER CODE END TIM3_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_OC_InitTypeDef sConfigOC = {0};

  /* USER CODE BEGIN TIM3_Init 1 */

  /* USER CODE END TIM3_Init 1 */
  htim3.Instance = TIM3;
  htim3.Init.Prescaler = 72 - 1;
  htim3.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim3.Init.Period = 10 - 1;
  htim3.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim3.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim3, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_PWM_Init(&htim3) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim3, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigOC.OCMode = TIM_OCMODE_PWM1;
  sConfigOC.Pulse = 5;
  sConfigOC.OCPolarity = TIM_OCPOLARITY_LOW;
  sConfigOC.OCFastMode = TIM_OCFAST_DISABLE;
  if (HAL_TIM_PWM_ConfigChannel(&htim3, &sConfigOC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM3_Init 2 */
  HAL_TIM_PWM_Start(&htim3, TIM_CHANNEL_2);       /* 开启对应PWM通道 */
  /* USER CODE END TIM3_Init 2 */
  HAL_TIM_MspPostInit(&htim3);

}
/* TIM8 init function */
void MX_TIM8_Init(void)
{

  /* USER CODE BEGIN TIM8_Init 0 */

  /* USER CODE END TIM8_Init 0 */

  TIM_ClockConfigTypeDef sClockSourceConfig = {0};
  TIM_SlaveConfigTypeDef sSlaveConfig = {0};
  TIM_MasterConfigTypeDef sMasterConfig = {0};
  TIM_IC_InitTypeDef sConfigIC = {0};

  /* USER CODE BEGIN TIM8_Init 1 */

  /* USER CODE END TIM8_Init 1 */
  htim8.Instance = TIM8;
  htim8.Init.Prescaler = 0;
  htim8.Init.CounterMode = TIM_COUNTERMODE_UP;
  htim8.Init.Period = 65535;
  htim8.Init.ClockDivision = TIM_CLOCKDIVISION_DIV1;
  htim8.Init.RepetitionCounter = 0;
  htim8.Init.AutoReloadPreload = TIM_AUTORELOAD_PRELOAD_ENABLE;
  if (HAL_TIM_Base_Init(&htim8) != HAL_OK)
  {
    Error_Handler();
  }
  sClockSourceConfig.ClockSource = TIM_CLOCKSOURCE_INTERNAL;
  if (HAL_TIM_ConfigClockSource(&htim8, &sClockSourceConfig) != HAL_OK)
  {
    Error_Handler();
  }
  if (HAL_TIM_IC_Init(&htim8) != HAL_OK)
  {
    Error_Handler();
  }
  sSlaveConfig.SlaveMode = TIM_SLAVEMODE_RESET;
  sSlaveConfig.InputTrigger = TIM_TS_TI1FP1;
  sSlaveConfig.TriggerPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sSlaveConfig.TriggerFilter = 0;
  if (HAL_TIM_SlaveConfigSynchro(&htim8, &sSlaveConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sMasterConfig.MasterOutputTrigger = TIM_TRGO_RESET;
  sMasterConfig.MasterSlaveMode = TIM_MASTERSLAVEMODE_DISABLE;
  if (HAL_TIMEx_MasterConfigSynchronization(&htim8, &sMasterConfig) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_RISING;
  sConfigIC.ICSelection = TIM_ICSELECTION_DIRECTTI;
  sConfigIC.ICPrescaler = TIM_ICPSC_DIV1;
  sConfigIC.ICFilter = 0;
  if (HAL_TIM_IC_ConfigChannel(&htim8, &sConfigIC, TIM_CHANNEL_1) != HAL_OK)
  {
    Error_Handler();
  }
  sConfigIC.ICPolarity = TIM_INPUTCHANNELPOLARITY_FALLING;
  sConfigIC.ICSelection = TIM_ICSELECTION_INDIRECTTI;
  if (HAL_TIM_IC_ConfigChannel(&htim8, &sConfigIC, TIM_CHANNEL_2) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN TIM8_Init 2 */
  __HAL_TIM_ENABLE_IT(&htim8, TIM_IT_UPDATE);
  HAL_TIM_IC_Start_IT(&htim8, TIM_CHANNEL_1);
  HAL_TIM_IC_Start_IT(&htim8, TIM_CHANNEL_2);
  /* USER CODE END TIM8_Init 2 */

}

void HAL_TIM_Base_MspInit(TIM_HandleTypeDef* tim_baseHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(tim_baseHandle->Instance==TIM3)
  {
  /* USER CODE BEGIN TIM3_MspInit 0 */

  /* USER CODE END TIM3_MspInit 0 */
    /* TIM3 clock enable */
    __HAL_RCC_TIM3_CLK_ENABLE();
  /* USER CODE BEGIN TIM3_MspInit 1 */

  /* USER CODE END TIM3_MspInit 1 */
  }
  else if(tim_baseHandle->Instance==TIM8)
  {
  /* USER CODE BEGIN TIM8_MspInit 0 */

  /* USER CODE END TIM8_MspInit 0 */
    /* TIM8 clock enable */
    __HAL_RCC_TIM8_CLK_ENABLE();

    __HAL_RCC_GPIOC_CLK_ENABLE();
    /**TIM8 GPIO Configuration
    PC6     ------> TIM8_CH1
    */
    GPIO_InitStruct.Pin = GPIO_PIN_6;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_PULLDOWN;
    HAL_GPIO_Init(GPIOC, &GPIO_InitStruct);

    /* TIM8 interrupt Init */
    HAL_NVIC_SetPriority(TIM8_UP_IRQn, 2, 1);
    HAL_NVIC_EnableIRQ(TIM8_UP_IRQn);
    HAL_NVIC_SetPriority(TIM8_CC_IRQn, 2, 1);
    HAL_NVIC_EnableIRQ(TIM8_CC_IRQn);
  /* USER CODE BEGIN TIM8_MspInit 1 */

  /* USER CODE END TIM8_MspInit 1 */
  }
}
void HAL_TIM_MspPostInit(TIM_HandleTypeDef* timHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(timHandle->Instance==TIM3)
  {
  /* USER CODE BEGIN TIM3_MspPostInit 0 */

  /* USER CODE END TIM3_MspPostInit 0 */

    __HAL_RCC_GPIOB_CLK_ENABLE();
    /**TIM3 GPIO Configuration
    PB5     ------> TIM3_CH2
    */
    GPIO_InitStruct.Pin = LED0_Pin;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(LED0_GPIO_Port, &GPIO_InitStruct);

    __HAL_AFIO_REMAP_TIM3_PARTIAL();

  /* USER CODE BEGIN TIM3_MspPostInit 1 */

  /* USER CODE END TIM3_MspPostInit 1 */
  }

}

void HAL_TIM_Base_MspDeInit(TIM_HandleTypeDef* tim_baseHandle)
{

  if(tim_baseHandle->Instance==TIM3)
  {
  /* USER CODE BEGIN TIM3_MspDeInit 0 */

  /* USER CODE END TIM3_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM3_CLK_DISABLE();
  /* USER CODE BEGIN TIM3_MspDeInit 1 */

  /* USER CODE END TIM3_MspDeInit 1 */
  }
  else if(tim_baseHandle->Instance==TIM8)
  {
  /* USER CODE BEGIN TIM8_MspDeInit 0 */

  /* USER CODE END TIM8_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_TIM8_CLK_DISABLE();

    /**TIM8 GPIO Configuration
    PC6     ------> TIM8_CH1
    */
    HAL_GPIO_DeInit(GPIOC, GPIO_PIN_6);

    /* TIM8 interrupt Deinit */
    HAL_NVIC_DisableIRQ(TIM8_UP_IRQn);
    HAL_NVIC_DisableIRQ(TIM8_CC_IRQn);
  /* USER CODE BEGIN TIM8_MspDeInit 1 */

  /* USER CODE END TIM8_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
/**
 * @brief       定时器TIMX PWM输入模式 重新启动捕获
 * @param       无
 * @retval      无
 */
void atim_timx_pwmin_chy_restart(void)
{
    sys_intx_disable();                           	/* 关闭中断 */

    g_timxchy_pwmin_sta = 0;                      	/* 清零状态,重新开始检测 */
    g_timxchy_pwmin_psc = 0;                       	/* 分频系数清零 */

    __HAL_TIM_SET_PRESCALER(&htim8, 0);           	/* 以最大的计数频率采集,以得到最好的精度 */
    __HAL_TIM_SET_COUNTER(&htim8, 0);             	/* 计数器清零 */

    __HAL_TIM_ENABLE_IT(&htim8, TIM_IT_CC1);      	/* 使能通道1捕获中断 */
    __HAL_TIM_ENABLE_IT(&htim8, TIM_IT_UPDATE);  	/* 使能溢出中断 */
    __HAL_TIM_ENABLE(&htim8);                     	/* 使能定时器TIMX */

    TIM8->SR = 0;                          			/* 清除所有中断标志位 */

    sys_intx_enable();                        		/* 打开中断 */
}

/**
 * @brief       定时器TIMX 通道Y PWM输入模式 中断处理函数
 * @note
 *              因为TIM1/TIM8等有多个中断服务函数,而TIM2~5/TIM12/TIM15等普通定时器只有1个中断服务
 *              函数,为了更好的兼容,我们对中断处理统一放到atim_timx_pwin_chy_process函数里面进行处理
 *
 * @param       无
 * @retval      无
 */
void atim_timx_pwmin_chy_process(void)
{
    static uint8_t sflag = 0;               				 /* 启动PWMIN输入检测标志 */

    if (g_timxchy_pwmin_sta)
    {
        g_timxchy_pwmin_psc = 0;
        TIM8->SR = 0;                                        /* 清除所有中断标志位 */
        __HAL_TIM_SET_COUNTER(&htim8, 0);             		 /* 计数器清零 */
        return ;
    }

    if (__HAL_TIM_GET_FLAG(&htim8, TIM_FLAG_UPDATE))         /* 发生了溢出中断 */
    {
        __HAL_TIM_CLEAR_FLAG(&htim8, TIM_FLAG_UPDATE);       /* 清除溢出中断标记 */

        if (__HAL_TIM_GET_FLAG(&htim8, TIM_FLAG_CC1) == 0)   /* 没有发生周期捕获中断,且捕获未完成 */
        {
            sflag = 0;
            if (g_timxchy_pwmin_psc == 0)   /* 从0 到 1 */
            {
                g_timxchy_pwmin_psc ++;
            }
            else
            {
                if (g_timxchy_pwmin_psc == 65535)               /* 已经最大了,可能是无输入状态 */
                {
                    g_timxchy_pwmin_psc = 0;                    /* 重新恢复不分频 */
                }
                else if (g_timxchy_pwmin_psc > 32767)           /* 不能倍增了 */
                {
                    g_timxchy_pwmin_psc = 65535;                /* 直接等于最大分频系数 */
                }
                else
                {
                    g_timxchy_pwmin_psc += g_timxchy_pwmin_psc; /* 倍增 */
                }
            }

            __HAL_TIM_SET_PRESCALER(&htim8, g_timxchy_pwmin_psc); /* 设置定时器预分频系数 */
            __HAL_TIM_SET_COUNTER(&htim8, 0);                     /* 计数器清零 */
            TIM8->SR = 0;                                   	  /* 清除所有中断标志位 */
            return ;
        }
    }

    if (sflag == 0)   /* 第一次采集到捕获中断 */
    {
        if (__HAL_TIM_GET_FLAG(&htim8, TIM_FLAG_CC1))             /* 检测到了第一次周期捕获中断 */
        {
            sflag = 1;  /* 标记第一次周期已经捕获, 第二次周期捕获可以开始了 */
        }

        TIM8->SR = 0;    /* 清除所有中断标志位 */
        return ;                    /* 完成此次操作 */
    }

    if (g_timxchy_pwmin_sta == 0)   /* 还没有成功捕获 */
    {
        if (__HAL_TIM_GET_FLAG(&htim8, TIM_FLAG_CC1))         	/* 检测到了周期捕获中断 */
        {
            g_timxchy_pwmin_hval = HAL_TIM_ReadCapturedValue(&htim8, TIM_CHANNEL_2) + 1; /* 高定平脉宽捕获值 */
            g_timxchy_pwmin_cval = HAL_TIM_ReadCapturedValue(&htim8, TIM_CHANNEL_1) + 1; /* 周期捕获值 */

            if (g_timxchy_pwmin_hval < g_timxchy_pwmin_cval)    /* 高电平脉宽必定小于周期长度 */
            {
                g_timxchy_pwmin_sta = 1;                        /* 标记捕获成功 */

                g_timxchy_pwmin_psc = TIM8->PSC;     		    /* 获取PWM输入分频系数 */

                if (g_timxchy_pwmin_psc == 0)                   /* 分频系数为0的时候, 修正读取数据 */
                {
                    g_timxchy_pwmin_hval++;                     /* 修正系数为1, 加1 */
                    g_timxchy_pwmin_cval++;                     /* 修正系数为1, 加1 */
                }

                sflag = 0;
                /* 每次捕获PWM输入成功后, 停止捕获,避免频繁中断影响系统正常代码运行 */
                TIM8->CR1  &= ~(1 << 0);                    	/* 关闭定时器TIMX */
                __HAL_TIM_DISABLE_IT(&htim8, TIM_IT_CC1);     	/* 关闭通道1捕获中断 */
                __HAL_TIM_DISABLE_IT(&htim8, TIM_IT_CC2);     	/* 关闭通道2捕获中断 */
                __HAL_TIM_DISABLE_IT(&htim8, TIM_IT_UPDATE);  	/* 关闭溢出中断 */

                TIM8->SR = 0;                                  	/* 清除所有中断标志位 */
            }
            else
            {
                atim_timx_pwmin_chy_restart();
            }
        }
    }

    TIM8->SR = 0;                                                    /* 清除所有中断标志位 */
}

/* USER CODE END 1 */
