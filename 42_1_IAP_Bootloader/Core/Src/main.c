/* USER CODE BEGIN Header */
/**
 ****************************************************************************************************
 * @file        main.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2024-01-01
 * @brief       串口IAP 实验-IAP Bootloader V1.0
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
#include "usart.h"
#include "gpio.h"
#include "fsmc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "../../BSP/LED/led.h"
#include "../../BSP/BEEP/beep.h"
#include "../../BSP/KEY/key.h"
#include "../../BSP/LCD/lcd.h"
#include "../../SYSTEM/delay/delay.h"
#include "../../SYSTEM/sys/sys.h"
#include "../../ATK_Middlewares/IAP/iap.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

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
    uint8_t t;
    uint8_t key;
    uint32_t oldcount = 0;      /* 老的串口接收数据值 */
    uint32_t applenth = 0;      /* 接收到的app代码长度 */
    uint8_t clearflag = 0;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  delay_init(72);						   /* 初始化延时函数 */
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_FSMC_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  lcd_init();                                         /* 初始化LCD */
  lcd_show_string(30,  50, 200, 16, 16, "STM32", RED);
  lcd_show_string(30,  70, 200, 16, 16, "IAP TEST", RED);
  lcd_show_string(30,  90, 200, 16, 16, "ATOM@ALIENTEK", RED);
  lcd_show_string(30, 110, 200, 16, 16, "KEY_UP: Copy APP2FLASH!", RED);
  lcd_show_string(30, 130, 200, 16, 16, "KEY1: Run FLASH APP", RED);
  lcd_show_string(30, 150, 200, 16, 16, "KEY0: Run SRAM APP", RED);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  if (g_usart_rx_cnt)
	  {
		  if (oldcount == g_usart_rx_cnt)   /* 新周期内,没有收到任何数据,认为本次数据接收完成 */
		  {
			  applenth = g_usart_rx_cnt;
			  oldcount = 0;
			  g_usart_rx_cnt = 0;
			  printf("用户程序接收完成!\r\n");
			  printf("代码长度:%luBytes\r\n", applenth);
		  }
		  else oldcount = g_usart_rx_cnt;
	  }

	  t++;
	  delay_ms(100);

	  if (t == 3)
	  {
		  LED0_TOGGLE();
		  t = 0;

		  if (clearflag)
		  {
			  clearflag--;

			  if (clearflag == 0)
			  {
				  lcd_fill(30, 190, 240, 210 + 16, WHITE);    /* 清除显示 */
			  }
		  }
	  }

	  key = key_scan(0);

	  if (key == WKUP_PRES)   /* WKUP按下,更新固件到FLASH */
	  {
		  if (applenth)
		  {
			  printf("开始更新固件...\r\n");
			  lcd_show_string(30, 190, 200, 16, 16, "Copying APP2FLASH...", BLUE);

			  if (((*(volatile uint32_t *)(0X20002000 + 4)) & 0xFF000000) == 0x08000000)  /* 判断是否为0X08XXXXXX */
			  {
				  iap_write_appbin(FLASH_APP1_ADDR, g_usart_rx_buf, applenth);            /* 更新FLASH代码 */
				  lcd_show_string(30, 190, 200, 16, 16, "Copy APP Successed!!", BLUE);
				  printf("固件更新完成!\r\n");
			  }
			  else
			  {
				  lcd_show_string(30, 190, 200, 16, 16, "Illegal FLASH APP!  ", BLUE);
				  printf("非FLASH应用程序!\r\n");
			  }
		  }
		  else
		  {
			  printf("没有可以更新的固件!\r\n");
			  lcd_show_string(30, 190, 200, 16, 16, "No APP!", BLUE);
		  }

		  clearflag = 7; /* 标志更新了显示,并且设置7*300ms后清除显示 */
	  }

	  if (key == KEY1_PRES)   /* KEY1按键按下, 运行FLASH APP代码 */
	  {
		  if (((*(volatile uint32_t *)(FLASH_APP1_ADDR + 4)) & 0xFF000000) == 0x08000000) /* 判断FLASH里面是否有APP,有的话执行 */
		  {
			  printf("开始执行FLASH用户代码!!\r\n\r\n");
			  delay_ms(10);
			  iap_load_app(FLASH_APP1_ADDR);/* 执行FLASH APP代码 */

		  }
		  else
		  {
			  printf("没有可以运行的固件!\r\n");
			  lcd_show_string(30, 190, 200, 16, 16, "No APP!", BLUE);
		  }

		  clearflag = 7; /* 标志更新了显示,并且设置7*300ms后清除显示 */
	  }

	  if (key == KEY0_PRES)   /* KEY0按下 */
	  {
		  printf("开始执行SRAM用户代码!!\r\n\r\n");
		  delay_ms(10);

		  if (((*(volatile uint32_t *)(0X20002000 + 4)) & 0xFF000000) == 0x20000000)   /* 判断是否为0X20XXXXXX */
		  {
			  iap_load_app(0X20002000);   /* SRAM地址 */
		  }
		  else
		  {
			  printf("非SRAM应用程序,无法执行!\r\n");
			  lcd_show_string(30, 190, 200, 16, 16, "Illegal SRAM APP!", BLUE);
		  }

		  clearflag = 7; /* 标志更新了显示,并且设置7*300ms后清除显示 */
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
