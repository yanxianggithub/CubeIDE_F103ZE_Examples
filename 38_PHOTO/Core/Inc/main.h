/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : main.h
  * @brief          : Header for main.c file.
  *                   This file contains the common defines of the application.
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
#ifndef __MAIN_H
#define __MAIN_H

#ifdef __cplusplus
extern "C" {
#endif

/* Includes ------------------------------------------------------------------*/
#include "stm32f1xx_hal.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */

/* USER CODE END Includes */

/* Exported types ------------------------------------------------------------*/
/* USER CODE BEGIN ET */

/* USER CODE END ET */

/* Exported constants --------------------------------------------------------*/
/* USER CODE BEGIN EC */

/* USER CODE END EC */

/* Exported macro ------------------------------------------------------------*/
/* USER CODE BEGIN EM */

/* USER CODE END EM */

/* Exported functions prototypes ---------------------------------------------*/
void Error_Handler(void);

/* USER CODE BEGIN EFP */

/* USER CODE END EFP */

/* Private defines -----------------------------------------------------------*/
#define KEY1_Pin GPIO_PIN_3
#define KEY1_GPIO_Port GPIOE
#define KEY0_Pin GPIO_PIN_4
#define KEY0_GPIO_Port GPIOE
#define LED1_Pin GPIO_PIN_5
#define LED1_GPIO_Port GPIOE
#define WK_UP_Pin GPIO_PIN_0
#define WK_UP_GPIO_Port GPIOA
#define LCD_BL_Pin GPIO_PIN_0
#define LCD_BL_GPIO_Port GPIOB
#define LCD_RS_Pin GPIO_PIN_0
#define LCD_RS_GPIO_Port GPIOG
#define NORFLASH_CS_Pin GPIO_PIN_12
#define NORFLASH_CS_GPIO_Port GPIOB
#define SD_D0_Pin GPIO_PIN_8
#define SD_D0_GPIO_Port GPIOC
#define OV7725_VSYNC_Pin GPIO_PIN_8
#define OV7725_VSYNC_GPIO_Port GPIOA
#define OV7725_VSYNC_EXTI_IRQn EXTI9_5_IRQn
#define SD_CLK_Pin GPIO_PIN_12
#define SD_CLK_GPIO_Port GPIOC
#define SD_CMD_Pin GPIO_PIN_2
#define SD_CMD_GPIO_Port GPIOD
#define SCCB_SCL_Pin GPIO_PIN_3
#define SCCB_SCL_GPIO_Port GPIOD
#define LCD_RD_Pin GPIO_PIN_4
#define LCD_RD_GPIO_Port GPIOD
#define LCD_WR_Pin GPIO_PIN_5
#define LCD_WR_GPIO_Port GPIOD
#define OV7725_WRST_Pin GPIO_PIN_6
#define OV7725_WRST_GPIO_Port GPIOD
#define LCD_CS_Pin GPIO_PIN_12
#define LCD_CS_GPIO_Port GPIOG
#define SCCB_SDA_Pin GPIO_PIN_13
#define SCCB_SDA_GPIO_Port GPIOG
#define OV7725_RRST_Pin GPIO_PIN_14
#define OV7725_RRST_GPIO_Port GPIOG
#define OV7725_OE_Pin GPIO_PIN_15
#define OV7725_OE_GPIO_Port GPIOG
#define OV7725_WEN_Pin GPIO_PIN_3
#define OV7725_WEN_GPIO_Port GPIOB
#define OV7725_RCLK_Pin GPIO_PIN_4
#define OV7725_RCLK_GPIO_Port GPIOB
#define LED0_Pin GPIO_PIN_5
#define LED0_GPIO_Port GPIOB
#define BEEP_Pin GPIO_PIN_8
#define BEEP_GPIO_Port GPIOB

/* USER CODE BEGIN Private defines */

/* USER CODE END Private defines */

#ifdef __cplusplus
}
#endif

#endif /* __MAIN_H */
