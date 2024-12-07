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

/* �ļ�ϵͳ���ⲿ FLASH ����ʼ��ַ
 * ��Ϊ������FATFSʵ������涨, FATFSռ��FLASH��ǰ12M�ֽڿռ�
 * ���, ����ַ��0, ���FATFSʵ��, diskio.c��˵��
 */
#define USB_STORAGE_FLASH_BASE  0


/* �Լ������һ�����USB״̬�ļĴ���, �����ж�USB״̬
 * bit0 : ��ʾ����������SD��д������
 * bit1 : ��ʾ��������SD����������
 * bit2 : SD��д���ݴ����־λ
 * bit3 : SD�������ݴ����־λ
 * bit4 : 1,��ʾ��������ѯ����(�������ӻ�������)
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
  'A', 'L', 'I', 'E', 'N', 'T', 'E', 'K', ' ',/* 9�ֽ� */
  /* Product Identification */
  'S', 'P', 'I', ' ', 'F', 'l', 'a', 's', 'h',/* 15�ֽ� */
  ' ', 'D', 'i', 's', 'k', ' ',
  /* Product Revision Level */
  '1', '.', '0', ' ',                         /* 4�ֽ� */


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
  'A', 'L', 'I', 'E', 'N', 'T', 'E', 'K', ' ',    /* 9�ֽ� */
  /* Product Identification */
  'S', 'D', ' ', 'F', 'l', 'a', 's', 'h', ' ',    /* 15�ֽ� */
  'D', 'i', 's', 'k', ' ', ' ',
  /* Product Revision Level */
  '1', '.', '0', ' ',                             /* 4�ֽ� */
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

        case 1: /* SD�� */
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
			*block_num = (1024 * 1024 * 12) / 512;   /* SPI FLASH�� 12MB �ֽ�, �ļ�ϵͳ�� */
			break;

		case 1: /* SD�� */
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
  g_usb_state_reg |= 0X10;    /* �����ѯ */
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
	g_usb_state_reg |= 0X02;    /* ������ڶ����� */

	switch (lun)
	{
		case 0: /* SPI FLASH */
			norflash_read(buf, USB_STORAGE_FLASH_BASE + blk_addr * 512, blk_len * 512);
			break;

		case 1: /* SD�� */
			res = sd_read_disk(buf, blk_addr, blk_len);
			break;
	}

	if (res)
	{
		printf("rerr:%d,%d\r\n", lun, res);
		g_usb_state_reg |= 0X08;    /* ������! */
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
	g_usb_state_reg |= 0X01;    /* �������д���� */

	switch (lun)
	{
		case 0: /* SPI FLASH */
			norflash_write(buf, USB_STORAGE_FLASH_BASE + blk_addr * 512, blk_len * 512);
			break;

		case 1: /* SD�� */
			res = sd_write_disk(buf, blk_addr, blk_len);
			break;
	}

	if (res)
	{
		g_usb_state_reg |= 0X04;    /* д����! */
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
    /* STORAGE_LUN_NBR ��usbd_conf.h���涨��, Ĭ����2 */
    if (hsd.SdCard.BlockNbr && hsd.SdCard.BlockSize)    /* ���SD������, ��֧��2������ */
    {
        return STORAGE_LUN_NBR - 1;
    }
    else    /* SD��������, ��ֻ֧��1������ */
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

