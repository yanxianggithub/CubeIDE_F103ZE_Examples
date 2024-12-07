/* USER CODE BEGIN Header */
/**
  ******************************************************************************
  * @file           : usbd_storage_if.c
  * @version        : v2.0_Cube
  * @brief          : Memory management layer.
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
#include "usbd_storage_if.h"

/* USER CODE BEGIN INCLUDE */
#include "../../BSP/NORFLASH/norflash.h"
#include "sdio.h"
/* USER CODE END INCLUDE */

/* Private typedef -----------------------------------------------------------*/
/* Private define ------------------------------------------------------------*/
/* Private macro -------------------------------------------------------------*/

/* USER CODE BEGIN PV */
/* Private variables ---------------------------------------------------------*/

/* 文件系统在外部 FLASH 的起始地址
 * 因为我们在FATFS实验里面规定, FATFS占用FLASH的前12M字节空间
 * 因此, 基地址是0, 详见FATFS实验, diskio.c的说明
 */
#define USB_STORAGE_FLASH_BASE  0


/* 自己定义的一个标记USB状态的寄存器, 方便判断USB状态
 * bit0 : 表示电脑正在向SD卡写入数据
 * bit1 : 表示电脑正从SD卡读出数据
 * bit2 : SD卡写数据错误标志位
 * bit3 : SD卡读数据错误标志位
 * bit4 : 1,表示电脑有轮询操作(表明连接还保持着)
 */
volatile uint8_t g_usb_state_reg = 0;

/* USER CODE END PV */

/** @addtogroup STM32_USB_OTG_DEVICE_LIBRARY
  * @brief Usb device.
  * @{
  */

/** @defgroup USBD_STORAGE
  * @brief Usb mass storage device module
  * @{
  */

/** @defgroup USBD_STORAGE_Private_TypesDefinitions
  * @brief Private types.
  * @{
  */

/* USER CODE BEGIN PRIVATE_TYPES */

/* USER CODE END PRIVATE_TYPES */

/**
  * @}
  */

/** @defgroup USBD_STORAGE_Private_Defines
  * @brief Private defines.
  * @{
  */

#define STORAGE_LUN_NBR                  2

/* USER CODE BEGIN PRIVATE_DEFINES */

/* USER CODE END PRIVATE_DEFINES */

/**
  * @}
  */

/** @defgroup USBD_STORAGE_Private_Macros
  * @brief Private macros.
  * @{
  */

/* USER CODE BEGIN PRIVATE_MACRO */

/* USER CODE END PRIVATE_MACRO */

/**
  * @}
  */

/** @defgroup USBD_STORAGE_Private_Variables
  * @brief Private variables.
  * @{
  */

/* USER CODE BEGIN INQUIRY_DATA_FS */
/** USB Mass storage Standard Inquiry Data. */
const int8_t STORAGE_Inquirydata_FS[] = {/* 36 */

  /* LUN 0 */
  0x00,
  0x80,
  0x02,
  0x02,
  (STANDARD_INQUIRY_DATA_LEN - 4),
  0x00,
  0x00,
  0x00,
  /* Vendor Identification */
  'A', 'L', 'I', 'E', 'N', 'T', 'E', 'K', ' ',/* 9字节 */
  /* Product Identification */
  'S', 'P', 'I', ' ', 'F', 'l', 'a', 's', 'h',/* 15字节 */
  ' ', 'D', 'i', 's', 'k', ' ',
  /* Product Revision Level */
  '1', '.', '0', ' ',                         /* 4字节 */


  /* LUN 1 */
  0x00,
  0x80,
  0x02,
  0x02,
  (STANDARD_INQUIRY_DATA_LEN - 4),
  0x00,
  0x00,
  0x00,
  /* Vendor Identification */
  'A', 'L', 'I', 'E', 'N', 'T', 'E', 'K', ' ',    /* 9字节 */
  /* Product Identification */
  'S', 'D', ' ', 'F', 'l', 'a', 's', 'h', ' ',    /* 15字节 */
  'D', 'i', 's', 'k', ' ', ' ',
  /* Product Revision Level */
  '1', '.', '0', ' ',                             /* 4字节 */
};
/* USER CODE END INQUIRY_DATA_FS */

/* USER CODE BEGIN PRIVATE_VARIABLES */

/* USER CODE END PRIVATE_VARIABLES */

/**
  * @}
  */

/** @defgroup USBD_STORAGE_Exported_Variables
  * @brief Public variables.
  * @{
  */

extern USBD_HandleTypeDef hUsbDeviceFS;

/* USER CODE BEGIN EXPORTED_VARIABLES */

/* USER CODE END EXPORTED_VARIABLES */

/**
  * @}
  */

/** @defgroup USBD_STORAGE_Private_FunctionPrototypes
  * @brief Private functions declaration.
  * @{
  */

static int8_t STORAGE_Init_FS(uint8_t lun);
static int8_t STORAGE_GetCapacity_FS(uint8_t lun, uint32_t *block_num, uint16_t *block_size);
static int8_t STORAGE_IsReady_FS(uint8_t lun);
static int8_t STORAGE_IsWriteProtected_FS(uint8_t lun);
static int8_t STORAGE_Read_FS(uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len);
static int8_t STORAGE_Write_FS(uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len);
static int8_t STORAGE_GetMaxLun_FS(void);

/* USER CODE BEGIN PRIVATE_FUNCTIONS_DECLARATION */

/* USER CODE END PRIVATE_FUNCTIONS_DECLARATION */

/**
  * @}
  */

USBD_StorageTypeDef USBD_Storage_Interface_fops_FS =
{
  STORAGE_Init_FS,
  STORAGE_GetCapacity_FS,
  STORAGE_IsReady_FS,
  STORAGE_IsWriteProtected_FS,
  STORAGE_Read_FS,
  STORAGE_Write_FS,
  STORAGE_GetMaxLun_FS,
  (int8_t *)STORAGE_Inquirydata_FS
};

/* Private functions ---------------------------------------------------------*/
/**
  * @brief  Initializes over USB FS IP
  * @param  lun:
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
int8_t STORAGE_Init_FS(uint8_t lun)
{
  /* USER CODE BEGIN 2 */
    uint8_t res = 0;

    switch (lun)
    {
        case 0: /* SPI FLASH */
            norflash_init();
            break;

        case 1: /* SD卡 */
        	MX_SDIO_SD_Init();
            break;
    }

    return res;
  /* USER CODE END 2 */
}

/**
  * @brief  .
  * @param  lun: .
  * @param  block_num: .
  * @param  block_size: .
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
int8_t STORAGE_GetCapacity_FS(uint8_t lun, uint32_t *block_num, uint16_t *block_size)
{
  /* USER CODE BEGIN 3 */
	switch (lun)
	{
		case 0: /* SPI FLASH */
			*block_size = 512;
			*block_num = (1024 * 1024 * 12) / 512;   /* SPI FLASH的 12MB 字节, 文件系统用 */
			break;

		case 1: /* SD卡 */
			*block_size = 512;
			*block_num = ((long long)hsd.SdCard.BlockNbr *hsd.SdCard.BlockSize)/512;//g_sd_card_info.CardCapacity / 512;
			break;
	}
  return (USBD_OK);
  /* USER CODE END 3 */
}

/**
  * @brief  .
  * @param  lun: .
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
int8_t STORAGE_IsReady_FS(uint8_t lun)
{
  /* USER CODE BEGIN 4 */
  g_usb_state_reg |= 0X10;    /* 标记轮询 */
  return (USBD_OK);
  /* USER CODE END 4 */
}

/**
  * @brief  .
  * @param  lun: .
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
int8_t STORAGE_IsWriteProtected_FS(uint8_t lun)
{
  /* USER CODE BEGIN 5 */
  return (USBD_OK);
  /* USER CODE END 5 */
}

/**
  * @brief  .
  * @param  lun: .
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
int8_t STORAGE_Read_FS(uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len)
{
  /* USER CODE BEGIN 6 */
	int8_t res = 0;
	g_usb_state_reg |= 0X02;    /* 标记正在读数据 */

	switch (lun)
	{
		case 0: /* SPI FLASH */
			norflash_read(buf, USB_STORAGE_FLASH_BASE + blk_addr * 512, blk_len * 512);
			break;

		case 1: /* SD卡 */
			res = sd_read_disk(buf, blk_addr, blk_len);
			break;
	}

	if (res)
	{
		printf("rerr:%d,%d\r\n", lun, res);
		g_usb_state_reg |= 0X08;    /* 读错误! */
	}

	return res;
  /* USER CODE END 6 */
}

/**
  * @brief  .
  * @param  lun: .
  * @retval USBD_OK if all operations are OK else USBD_FAIL
  */
int8_t STORAGE_Write_FS(uint8_t lun, uint8_t *buf, uint32_t blk_addr, uint16_t blk_len)
{
  /* USER CODE BEGIN 7 */
	int8_t res = 0;
	g_usb_state_reg |= 0X01;    /* 标记正在写数据 */

	switch (lun)
	{
		case 0: /* SPI FLASH */
			norflash_write(buf, USB_STORAGE_FLASH_BASE + blk_addr * 512, blk_len * 512);
			break;

		case 1: /* SD卡 */
			res = sd_write_disk(buf, blk_addr, blk_len);
			break;
	}

	if (res)
	{
		g_usb_state_reg |= 0X04;    /* 写错误! */
		printf("werr:%d,%d\r\n", lun, res);
	}

	return res;
  /* USER CODE END 7 */
}

/**
  * @brief  .
  * @param  None
  * @retval .
  */
int8_t STORAGE_GetMaxLun_FS(void)
{
  /* USER CODE BEGIN 8 */
    /* STORAGE_LUN_NBR 在usbd_conf.h里面定义, 默认是2 */
    if (hsd.SdCard.BlockNbr && hsd.SdCard.BlockSize)    /* 如果SD卡正常, 则支持2个磁盘 */
    {
        return STORAGE_LUN_NBR - 1;
    }
    else    /* SD卡不正常, 则只支持1个磁盘 */
    {
        return STORAGE_LUN_NBR - 2;
    }
  /* USER CODE END 8 */
}

/* USER CODE BEGIN PRIVATE_FUNCTIONS_IMPLEMENTATION */

/* USER CODE END PRIVATE_FUNCTIONS_IMPLEMENTATION */

/**
  * @}
  */

/**
  * @}
  */

