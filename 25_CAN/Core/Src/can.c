/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file    can.c
  * @brief   This file provides code for the configuration
  *          of the CAN instances.
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
#include "can.h"

/* USER CODE BEGIN 0 */
CAN_TxHeaderTypeDef g_canx_txheader;    /* 发送参数句柄 */
CAN_RxHeaderTypeDef g_canx_rxheader;    /* 接收参数句柄 */
CAN_FilterTypeDef sFilterConfig;
/* USER CODE END 0 */

CAN_HandleTypeDef hcan;

/* CAN init function */
void MX_CAN_Init(void)
{

  /* USER CODE BEGIN CAN_Init 0 */

  /* USER CODE END CAN_Init 0 */

  /* USER CODE BEGIN CAN_Init 1 */

  /* USER CODE END CAN_Init 1 */
  hcan.Instance = CAN1;
  hcan.Init.Prescaler = 4;
  hcan.Init.Mode = CAN_MODE_LOOPBACK;
  hcan.Init.SyncJumpWidth = CAN_SJW_1TQ;
  hcan.Init.TimeSeg1 = CAN_BS1_9TQ;
  hcan.Init.TimeSeg2 = CAN_BS2_8TQ;
  hcan.Init.TimeTriggeredMode = DISABLE;
  hcan.Init.AutoBusOff = DISABLE;
  hcan.Init.AutoWakeUp = DISABLE;
  hcan.Init.AutoRetransmission = ENABLE;
  hcan.Init.ReceiveFifoLocked = DISABLE;
  hcan.Init.TransmitFifoPriority = DISABLE;
  if (HAL_CAN_Init(&hcan) != HAL_OK)
  {
    Error_Handler();
  }
  /* USER CODE BEGIN CAN_Init 2 */
  /*配置CAN过滤器*/
  sFilterConfig.FilterBank = 0;                         	/* 过滤器0 */
  sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;     	/* 标识符屏蔽位模式 */
  sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;    	/* 长度32位位宽*/
  sFilterConfig.FilterIdHigh = 0x0000;                  	/* 32位ID */
  sFilterConfig.FilterIdLow = 0x0000;
  sFilterConfig.FilterMaskIdHigh = 0x0000;              	/* 32位MASK */
  sFilterConfig.FilterMaskIdLow = 0x0000;
  sFilterConfig.FilterFIFOAssignment = CAN_FILTER_FIFO0;	/* 过滤器0关联到FIFO0 */
  sFilterConfig.FilterActivation = CAN_FILTER_ENABLE; 		/* 激活滤波器0 */
  sFilterConfig.SlaveStartFilterBank = 14;

  /* 过滤器配置 */
  if (HAL_CAN_ConfigFilter(&hcan, &sFilterConfig) != HAL_OK)
  {

  }

  /* 启动CAN外围设备 */
  if (HAL_CAN_Start(&hcan) != HAL_OK)
  {

  }
  /* USER CODE END CAN_Init 2 */

}

void HAL_CAN_MspInit(CAN_HandleTypeDef* canHandle)
{

  GPIO_InitTypeDef GPIO_InitStruct = {0};
  if(canHandle->Instance==CAN1)
  {
  /* USER CODE BEGIN CAN1_MspInit 0 */

  /* USER CODE END CAN1_MspInit 0 */
    /* CAN1 clock enable */
    __HAL_RCC_CAN1_CLK_ENABLE();

    __HAL_RCC_GPIOA_CLK_ENABLE();
    /**CAN GPIO Configuration
    PA11     ------> CAN_RX
    PA12     ------> CAN_TX
    */
    GPIO_InitStruct.Pin = GPIO_PIN_11;
    GPIO_InitStruct.Mode = GPIO_MODE_INPUT;
    GPIO_InitStruct.Pull = GPIO_NOPULL;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

    GPIO_InitStruct.Pin = GPIO_PIN_12;
    GPIO_InitStruct.Mode = GPIO_MODE_AF_PP;
    GPIO_InitStruct.Speed = GPIO_SPEED_FREQ_HIGH;
    HAL_GPIO_Init(GPIOA, &GPIO_InitStruct);

  /* USER CODE BEGIN CAN1_MspInit 1 */

  /* USER CODE END CAN1_MspInit 1 */
  }
}

void HAL_CAN_MspDeInit(CAN_HandleTypeDef* canHandle)
{

  if(canHandle->Instance==CAN1)
  {
  /* USER CODE BEGIN CAN1_MspDeInit 0 */

  /* USER CODE END CAN1_MspDeInit 0 */
    /* Peripheral clock disable */
    __HAL_RCC_CAN1_CLK_DISABLE();

    /**CAN GPIO Configuration
    PA11     ------> CAN_RX
    PA12     ------> CAN_TX
    */
    HAL_GPIO_DeInit(GPIOA, GPIO_PIN_11|GPIO_PIN_12);

  /* USER CODE BEGIN CAN1_MspDeInit 1 */

  /* USER CODE END CAN1_MspDeInit 1 */
  }
}

/* USER CODE BEGIN 1 */
/**
 * @brief       CAN 模式设置
 * @note      	模式选择：环回模式、普通模式
 * @param       mode     : CAN_MODE_NORMAL,  正常模式;
 *                         CAN_MODE_LOOPBACK,回环模式;
 * @retval      无
 */
void can_set_mode(uint32_t mode)
{
	hcan.Instance = CAN1;
	hcan.Init.Prescaler = 4;
	hcan.Init.Mode = mode;
	hcan.Init.SyncJumpWidth = CAN_SJW_1TQ;
	hcan.Init.TimeSeg1 = CAN_BS1_9TQ;
	hcan.Init.TimeSeg2 = CAN_BS2_8TQ;
	hcan.Init.TimeTriggeredMode = DISABLE;
	hcan.Init.AutoBusOff = DISABLE;
	hcan.Init.AutoWakeUp = DISABLE;
	hcan.Init.AutoRetransmission = ENABLE;
	hcan.Init.ReceiveFifoLocked = DISABLE;
	hcan.Init.TransmitFifoPriority = DISABLE;
	if (HAL_CAN_Init(&hcan) != HAL_OK)
	{
		Error_Handler();
	}
	/*配置CAN过滤器*/
	sFilterConfig.FilterBank = 0;                         	/* 过滤器0 */
	sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;     	/* 标识符屏蔽位模式 */
	sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;    	/* 长度32位位宽*/
	sFilterConfig.FilterIdHigh = 0x0000;                  	/* 32位ID */
	sFilterConfig.FilterIdLow = 0x0000;
	sFilterConfig.FilterMaskIdHigh = 0x0000;              	/* 32位MASK */
	sFilterConfig.FilterMaskIdLow = 0x0000;
	sFilterConfig.FilterFIFOAssignment = CAN_FILTER_FIFO0;	/* 过滤器0关联到FIFO0 */
	sFilterConfig.FilterActivation = CAN_FILTER_ENABLE; 		/* 激活滤波器0 */
	sFilterConfig.SlaveStartFilterBank = 14;

	/* 过滤器配置 */
	if (HAL_CAN_ConfigFilter(&hcan, &sFilterConfig) != HAL_OK)
	{

	}

	/* 启动CAN外围设备 */
	if (HAL_CAN_Start(&hcan) != HAL_OK)
	{

	}
}


/**
 * @brief       CAN 发送一组数据
 *   @note      发送格式固定为: 标准ID, 数据帧
 * @param       id      : 标准ID(11位)
 * @retval      发送状态 0, 成功; 1, 失败;
 */
uint8_t can_send_msg(uint32_t id, uint8_t *msg, uint8_t len)
{
  uint32_t TxMailbox = CAN_TX_MAILBOX0;

  g_canx_txheader.StdId = id;         /* 标准标识符 */
  g_canx_txheader.ExtId = id;         /* 扩展标识符(29位) 标准标识符情况下，该成员无效*/
  g_canx_txheader.IDE = CAN_ID_STD;   /* 使用标准标识符 */
  g_canx_txheader.RTR = CAN_RTR_DATA; /* 数据帧 */
  g_canx_txheader.DLC = len;

  if (HAL_CAN_AddTxMessage(&hcan, &g_canx_txheader, msg, &TxMailbox) != HAL_OK) /* 发送消息 */
  {
    return 1;
  }

  while (HAL_CAN_GetTxMailboxesFreeLevel(&hcan) != 3); /* 等待发送完成,所有邮箱(有三个邮箱)为空 */

  return 0;
}

/**
 * @brief       CAN 接收数据查询
 *   @note      接收数据格式固定为: 标准ID, 数据帧
 * @param       id      : 要查询的 标准ID(11位)
 * @param       buf     : 数据缓存区
 * @retval      接收结果
 *   @arg       0   , 无数据被接收到;
 *   @arg       其他, 接收的数据长度
 */
uint8_t can_receive_msg(uint32_t id, uint8_t *buf)
{
  if (HAL_CAN_GetRxFifoFillLevel(&hcan, CAN_RX_FIFO0) == 0)     /* 没有接收到数据 */
  {
    return 0;
  }

  if (HAL_CAN_GetRxMessage(&hcan, CAN_RX_FIFO0, &g_canx_rxheader, buf) != HAL_OK)  /* 读取数据 */
  {
    return 0;
  }

  if (g_canx_rxheader.StdId!= id || g_canx_rxheader.IDE != CAN_ID_STD || g_canx_rxheader.RTR != CAN_RTR_DATA)       /* 接收到的ID不对 / 不是标准帧 / 不是数据帧 */
  {
    return 0;
  }

  return g_canx_rxheader.DLC;
}


/* USER CODE END 1 */
