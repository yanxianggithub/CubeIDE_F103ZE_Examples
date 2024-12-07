/* USER CODE BEGIN Header */
/**
 ****************************************************************************************************
 * @file        main.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2024-01-01
 * @brief       USB读卡器 实验
 * @license     Copyright (c) 2020-2032, 广州市星翼电子科技有限公司
 ****************************************************************************************************
 * @attention
 *
 * 实验平台:正点原子 STM32F103开发板
 * 在线视频:www.yuanzige.com
 * 技术论坛:www.openedv.com
 * 公司网址:www.alientek.com
 * 购买地址:openedv.taobao.com
 *
 ****************************************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "sdio.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
#include "usb_device.h"
#include "gpio.h"
#include "fsmc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "../../BSP/LED/led.h"
#include "../../BSP/KEY/key.h"
#include "../../BSP/LCD/lcd.h"
#include "../../BSP/NORFLASH/norflash.h"
#include "../../SYSTEM/delay/delay.h"
#include "../../ATK_Middlewares/MALLOC/malloc.h"
#include "usbd_desc.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
extern uint8_t g_sd_sta;
extern volatile uint8_t g_usb_state_reg;    /* USB状态 */
extern volatile uint8_t g_device_state;     /* USB连接 情况 */
/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/* USER CODE END PM */

/* Private variables ---------------------------------------------------------*/

/* USER CODE BEGIN PV */

/* USER CODE END PV */

/* Private function prototypes -----------------------------------------------*/
void SystemClock_Config(void);
/* USER CODE BEGIN PFP */

/* USER CODE END PFP */

/* Private user code ---------------------------------------------------------*/
/* USER CODE BEGIN 0 */

/* USER CODE END 0 */

/**
  * @brief  The application entry point.
  * @retval int
  */
int main(void)
{
  /* USER CODE BEGIN 1 */
    uint8_t offline_cnt = 0;
    uint8_t tct = 0;
    uint8_t usb_sta;
    uint8_t device_sta;
    uint16_t id;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  delay_init(72);						   	/* 初始化延时函数 */
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_FSMC_Init();
  MX_USART1_UART_Init();
  MX_SDIO_SD_Init();
  MX_SPI2_Init();
  MX_TIM4_Init();
  MX_USB_DEVICE_Init();
  /* USER CODE BEGIN 2 */
  lcd_init();                 /* 初始化LCD */
  norflash_init();            /* 初始化NOR FLASH */
  my_mem_init(SRAMIN);        /* 初始化内部SRAM内存池 */

  lcd_show_string(30, 50, 200, 16, 16, "STM32", RED);
  lcd_show_string(30, 70, 200, 16, 16, "USB Card Reader TEST", RED);
  lcd_show_string(30, 90, 200, 16, 16, "ATOM@ALIENTEK", RED);

  if (g_sd_sta == 1)  /* 初始化SD卡 */
  {
	  g_sd_sta = 0;
	  MX_SDIO_SD_Init();
	  lcd_show_string(30, 110, 200, 16, 16, "SD Card Error!", RED);   /* 检测SD卡错误 */
  }
  else     /* SD 卡正常 */
  {
	  lcd_show_string(30, 110, 200, 16, 16, "SD Card Size:     MB", RED);
	  lcd_show_num(134, 110, SD_TOTAL_SIZE_MB(&hsd), 5, 16, RED);  /* 显示SD卡容量 */
  }

  id = norflash_read_id();
  if ((id == 0) || (id == 0XFFFF))
  {
	  lcd_show_string(30, 110, 200, 16, 16, "NorFlash Error!", RED);  /* 检测NorFlash错误 */
  }
  else   /* SPI FLASH 正常 */
  {
	  lcd_show_string(30, 130, 200, 16, 16, "SPI FLASH Size:7.25MB", RED);
  }

  usbd_port_config(0);    /* USB先断开 */
  delay_ms(500);
  usbd_port_config(1);    /* USB再次连接 */
  delay_ms(500);

  lcd_show_string(30, 170, 200, 16, 16, "USB Connecting...", RED);    /* 提示正在建立连接 */
  delay_ms(1800);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  delay_ms(1);

	  if (usb_sta != g_usb_state_reg)   /* 状态改变了 */
	  {
		  lcd_fill(30, 190, 240, 210 + 16, WHITE); /* 清除显示 */

		  if (g_usb_state_reg & 0x01)   /* 正在写 */
		  {
			  LED1(0);
			  lcd_show_string(30, 190, 200, 16, 16, "USB Writing...", RED); /* 提示USB正在写入数据 */
		  }

		  if (g_usb_state_reg & 0x02)   /* 正在读 */
		  {
			  LED1(0);
			  lcd_show_string(30, 190, 200, 16, 16, "USB Reading...", RED); /* 提示USB正在读出数据 */
		  }

		  if (g_usb_state_reg & 0x04)
		  {
			  lcd_show_string(30, 210, 200, 16, 16, "USB Write Err ", RED); /* 提示写入错误 */
		  }
		  else
		  {
			  lcd_fill(30, 210, 240, 230 + 16, WHITE); /* 清除显示 */
		  }

		  if (g_usb_state_reg & 0x08)
		  {
			  lcd_show_string(30, 230, 200, 16, 16, "USB Read  Err ", RED); /* 提示读出错误 */
		  }
		  else
		  {
			  lcd_fill(30, 230, 240, 250 + 16, WHITE); /* 清除显示 */
		  }

		  usb_sta = g_usb_state_reg; /* 记录最后的状态 */
	  }

	  if (device_sta != g_device_state)
	  {
		  if (g_device_state == 1)
		  {
			  lcd_show_string(30, 170, 200, 16, 16, "USB Connected    ", RED);    /* 提示USB连接已经建立 */
		  }
		  else
		  {
			  lcd_show_string(30, 170, 200, 16, 16, "USB DisConnected ", RED);    /* 提示USB被拔出了 */
		  }

		  device_sta = g_device_state;
	  }

	  tct++;

	  if (tct == 200)
	  {
		  tct = 0;
		  LED1(1);        /* 关闭 LED1 */
		  LED0_TOGGLE();  /* LED0 闪烁 */

		  if (g_usb_state_reg & 0x10)
		  {
			  offline_cnt = 0;    /* USB连接了,则清除offline计数器 */
			  g_device_state = 1;
		  }
		  else    /* 没有得到轮询 */
		  {
			  offline_cnt++;

			  if (offline_cnt > 100)
			  {
				  g_device_state = 0;/* 20s内没收到在线标记,代表USB被拔出了 */
			  }
		  }

		  g_usb_state_reg = 0;
	  }


    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  /* USER CODE END 3 */
}

/**
  * @brief System Clock Configuration
  * @retval None
  */
void SystemClock_Config(void)
{
  RCC_OscInitTypeDef RCC_OscInitStruct = {0};
  RCC_ClkInitTypeDef RCC_ClkInitStruct = {0};
  RCC_PeriphCLKInitTypeDef PeriphClkInit = {0};

  /** Initializes the RCC Oscillators according to the specified parameters
  * in the RCC_OscInitTypeDef structure.
  */
  RCC_OscInitStruct.OscillatorType = RCC_OSCILLATORTYPE_HSE;
  RCC_OscInitStruct.HSEState = RCC_HSE_ON;
  RCC_OscInitStruct.HSEPredivValue = RCC_HSE_PREDIV_DIV1;
  RCC_OscInitStruct.HSIState = RCC_HSI_ON;
  RCC_OscInitStruct.PLL.PLLState = RCC_PLL_ON;
  RCC_OscInitStruct.PLL.PLLSource = RCC_PLLSOURCE_HSE;
  RCC_OscInitStruct.PLL.PLLMUL = RCC_PLL_MUL9;
  if (HAL_RCC_OscConfig(&RCC_OscInitStruct) != HAL_OK)
  {
    Error_Handler();
  }

  /** Initializes the CPU, AHB and APB buses clocks
  */
  RCC_ClkInitStruct.ClockType = RCC_CLOCKTYPE_HCLK|RCC_CLOCKTYPE_SYSCLK
                              |RCC_CLOCKTYPE_PCLK1|RCC_CLOCKTYPE_PCLK2;
  RCC_ClkInitStruct.SYSCLKSource = RCC_SYSCLKSOURCE_PLLCLK;
  RCC_ClkInitStruct.AHBCLKDivider = RCC_SYSCLK_DIV1;
  RCC_ClkInitStruct.APB1CLKDivider = RCC_HCLK_DIV2;
  RCC_ClkInitStruct.APB2CLKDivider = RCC_HCLK_DIV1;

  if (HAL_RCC_ClockConfig(&RCC_ClkInitStruct, FLASH_LATENCY_2) != HAL_OK)
  {
    Error_Handler();
  }
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_USB;
  PeriphClkInit.UsbClockSelection = RCC_USBCLKSOURCE_PLL_DIV1_5;
  if (HAL_RCCEx_PeriphCLKConfig(&PeriphClkInit) != HAL_OK)
  {
    Error_Handler();
  }
}

/* USER CODE BEGIN 4 */

/* USER CODE END 4 */

/**
  * @brief  This function is executed in case of error occurrence.
  * @retval None
  */
void Error_Handler(void)
{
  /* USER CODE BEGIN Error_Handler_Debug */
  /* User can add his own implementation to report the HAL error return state */
  __disable_irq();
  while (1)
  {
  }
  /* USER CODE END Error_Handler_Debug */
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t *file, uint32_t line)
{
  /* USER CODE BEGIN 6 */
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */
  /* USER CODE END 6 */
}
#endif /* USE_FULL_ASSERT */
