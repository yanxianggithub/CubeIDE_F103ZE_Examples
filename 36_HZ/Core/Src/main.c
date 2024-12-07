/* USER CODE BEGIN Header */
/**
 ****************************************************************************************************
 * @file        main.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2024-01-01
 * @brief       汉字显示 实验
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
#include "fatfs.h"
#include "sdio.h"
#include "spi.h"
#include "tim.h"
#include "usart.h"
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
#include "../../FatFs/exfuns/exfuns.h"
#include "../../ATK_Middlewares/TEXT/text.h"
#include "ff.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
extern uint8_t g_sd_sta;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
uint8_t read_buf[50] = {0};
uint8_t write_buf[50] = "Hello test demo";

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
    uint32_t fontcnt;
    uint8_t i, j;
    uint8_t fontx[2]; /* GBK码 */
    uint8_t key, t;
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
  MX_FATFS_Init();
  MX_SPI2_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */
  lcd_init();                         		/* 初始化LCD */
  norflash_init();                    		/* 初始化NORFLASH */
  my_mem_init(SRAMIN);               		/* 初始化内部SRAM内存池 */
  exfuns_init();                 			/* 为fatfs相关变量申请内存 */

  f_mount(fs[0], "0:", 1);       			/* 挂载SD卡 */
  f_mount(fs[1], "1:", 1);       			/* 挂载SPI_FLASH卡 */

  while (fonts_init())                		/* 检查字库 */
  {
  UPD:
	  lcd_clear(WHITE);               		/* 清屏 */
	  lcd_show_string(30, 30, 200, 16, 16, "STM32", RED);

	  while (g_sd_sta == 1)               	/* 检测SD卡 */
	  {
		  g_sd_sta = 0;
		  MX_SDIO_SD_Init();
		  lcd_show_string(30, 50, 200, 16, 16, "SD Card Failed!", RED);
		  delay_ms(200);
		  lcd_fill(30, 50, 200 + 30, 50 + 16, WHITE);
		  delay_ms(200);
	  }

	  lcd_show_string(30, 50, 200, 16, 16, "SD Card OK", RED);
	  lcd_show_string(30, 70, 200, 16, 16, "Font Updating...", RED);
	  key = fonts_update_font(20, 90, 16, (uint8_t *)"0:", RED); /* 更新字库 */

	  while (key) /* 更新失败 */
	  {
		  lcd_show_string(30, 90, 200, 16, 16, "Font Update Failed!", RED);
		  delay_ms(200);
		  lcd_fill(20, 90, 200 + 20, 90 + 16, WHITE);
		  delay_ms(200);
	  }

	  lcd_show_string(30, 90, 200, 16, 16, "Font Update Success!   ", RED);
	  delay_ms(1500);
	  lcd_clear(WHITE); /* 清屏 */
  }

  text_show_string(30, 30, 200, 16, "正点原子STM32开发板", 16, 0, RED);
  text_show_string(30, 50, 200, 16, "GBK字库测试程序", 16, 0, RED);
  text_show_string(30, 70, 200, 16, "正点原子@ALIENTEK", 16, 0, RED);
  text_show_string(30, 90, 200, 16, "按KEY0,更新字库", 16, 0, RED);


  text_show_string(30, 110, 200, 16, "内码高字节:", 16, 0, BLUE);
  text_show_string(30, 130, 200, 16, "内码低字节:", 16, 0, BLUE);
  text_show_string(30, 150, 200, 16, "汉字计数器:", 16, 0, BLUE);

  text_show_string(30, 180, 200, 24, "对应汉字为:", 24, 0, BLUE);
  text_show_string(30, 204, 200, 16, "对应汉字(16*16)为:", 16, 0, BLUE);
  text_show_string(30, 220, 200, 16, "对应汉字(12*12)为:", 12, 0, BLUE);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  fontcnt = 0;

	  for (i = 0x81; i < 0xff; i++)               /* GBK内码高字节范围为0X81~0XFE */
	  {
		  fontx[0] = i;
		  lcd_show_num(118, 130, i, 3, 16, BLUE); /* 显示内码高字节 */

		  for (j = 0x40; j < 0xfe; j++)           /* GBK内码低字节范围为 0X40~0X7E, 0X80~0XFE) */
		  {
			  if (j == 0x7f)
				  continue;

			  fontcnt++;
			  lcd_show_num(118, 130, j, 3, 16, BLUE);       /* 显示内码低字节 */
			  lcd_show_num(118, 150, fontcnt, 5, 16, BLUE); /* 汉字计数显示 */
			  fontx[1] = j;
			  text_show_font(30 + 132, 180, fontx, 24, 0, BLUE);
			  text_show_font(30 + 144, 204, fontx, 16, 0, BLUE);
			  text_show_font(30 + 108, 220, fontx, 12, 0, BLUE);
			  t = 200;

			  while (t--)         /* 延时,同时扫描按键 */
			  {
				  delay_ms(1);
				  key = key_scan(0);

				  if (key == KEY0_PRES)
				  {
					  goto UPD;   /* 跳转到UPD位置(强制更新字库) */
				  }
			  }

			  LED0_TOGGLE();
		  }
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
