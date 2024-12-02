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
CAN_TxHeaderTypeDef g_canx_txheader;    /* ���Ͳ������ */
CAN_RxHeaderTypeDef g_canx_rxheader;    /* ���ղ������ */
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
  /*����CAN������*/
  sFilterConfig.FilterBank = 0;                         	/* ������0 */
  sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;     	/* ��ʶ������λģʽ */
  sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;    	/* ����32λλ��*/
  sFilterConfig.FilterIdHigh = 0x0000;                  	/* 32λID */
  sFilterConfig.FilterIdLow = 0x0000;
  sFilterConfig.FilterMaskIdHigh = 0x0000;              	/* 32λMASK */
  sFilterConfig.FilterMaskIdLow = 0x0000;
  sFilterConfig.FilterFIFOAssignment = CAN_FILTER_FIFO0;	/* ������0������FIFO0 */
  sFilterConfig.FilterActivation = CAN_FILTER_ENABLE; 		/* �����˲���0 */
  sFilterConfig.SlaveStartFilterBank = 14;

  /* ���������� */
  if (HAL_CAN_ConfigFilter(&hcan, &sFilterConfig) != HAL_OK)
  {

  }

  /* ����CAN��Χ�豸 */
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
 * @brief       CAN ģʽ����
 * @note      	ģʽѡ�񣺻���ģʽ����ͨģʽ
 * @param       mode     : CAN_MODE_NORMAL,  ����ģʽ;
 *                         CAN_MODE_LOOPBACK,�ػ�ģʽ;
 * @retval      ��
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
	/*����CAN������*/
	sFilterConfig.FilterBank = 0;                         	/* ������0 */
	sFilterConfig.FilterMode = CAN_FILTERMODE_IDMASK;     	/* ��ʶ������λģʽ */
	sFilterConfig.FilterScale = CAN_FILTERSCALE_32BIT;    	/* ����32λλ��*/
	sFilterConfig.FilterIdHigh = 0x0000;                  	/* 32λID */
	sFilterConfig.FilterIdLow = 0x0000;
	sFilterConfig.FilterMaskIdHigh = 0x0000;              	/* 32λMASK */
	sFilterConfig.FilterMaskIdLow = 0x0000;
	sFilterConfig.FilterFIFOAssignment = CAN_FILTER_FIFO0;	/* ������0������FIFO0 */
	sFilterConfig.FilterActivation = CAN_FILTER_ENABLE; 		/* �����˲���0 */
	sFilterConfig.SlaveStartFilterBank = 14;

	/* ���������� */
	if (HAL_CAN_ConfigFilter(&hcan, &sFilterConfig) != HAL_OK)
	{

	}

	/* ����CAN��Χ�豸 */
	if (HAL_CAN_Start(&hcan) != HAL_OK)
	{

	}
}


/**
 * @brief       CAN ����һ������
 *   @note      ���͸�ʽ�̶�Ϊ: ��׼ID, ����֡
 * @param       id      : ��׼ID(11λ)
 * @retval      ����״̬ 0, �ɹ�; 1, ʧ��;
 */
uint8_t can_send_msg(uint32_t id, uint8_t *msg, uint8_t len)
{
  uint32_t TxMailbox = CAN_TX_MAILBOX0;

  g_canx_txheader.StdId = id;         /* ��׼��ʶ�� */
  g_canx_txheader.ExtId = id;         /* ��չ��ʶ��(29λ) ��׼��ʶ������£��ó�Ա��Ч*/
  g_canx_txheader.IDE = CAN_ID_STD;   /* ʹ�ñ�׼��ʶ�� */
  g_canx_txheader.RTR = CAN_RTR_DATA; /* ����֡ */
  g_canx_txheader.DLC = len;

  if (HAL_CAN_AddTxMessage(&hcan, &g_canx_txheader, msg, &TxMailbox) != HAL_OK) /* ������Ϣ */
  {
    return 1;
  }

  while (HAL_CAN_GetTxMailboxesFreeLevel(&hcan) != 3); /* �ȴ��������,��������(����������)Ϊ�� */

  return 0;
}

/**
 * @brief       CAN �������ݲ�ѯ
 *   @note      �������ݸ�ʽ�̶�Ϊ: ��׼ID, ����֡
 * @param       id      : Ҫ��ѯ�� ��׼ID(11λ)
 * @param       buf     : ���ݻ�����
 * @retval      ���ս��
 *   @arg       0   , �����ݱ����յ�;
 *   @arg       ����, ���յ����ݳ���
 */
uint8_t can_receive_msg(uint32_t id, uint8_t *buf)
{
  if (HAL_CAN_GetRxFifoFillLevel(&hcan, CAN_RX_FIFO0) == 0)     /* û�н��յ����� */
  {
    return 0;
  }

  if (HAL_CAN_GetRxMessage(&hcan, CAN_RX_FIFO0, &g_canx_rxheader, buf) != HAL_OK)  /* ��ȡ���� */
  {
    return 0;
  }

  if (g_canx_rxheader.StdId!= id || g_canx_rxheader.IDE != CAN_ID_STD || g_canx_rxheader.RTR != CAN_RTR_DATA)       /* ���յ���ID���� / ���Ǳ�׼֡ / ��������֡ */
  {
    return 0;
  }

  return g_canx_rxheader.DLC;
}


/* USER CODE END 1 */
