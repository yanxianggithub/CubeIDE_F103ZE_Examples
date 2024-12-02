/* USER CODE BEGIN Header */

/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "spi.h"
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
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* 要写入到FLASH的字符串数组 */
const uint8_t g_text_buf[] = {"STM32 SPI TEST"};

#define TEXT_SIZE sizeof(g_text_buf) /* TEXT字符串长度 */

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
    uint8_t key;
    uint16_t i = 0;
    uint8_t datatemp[TEXT_SIZE+2];
    uint32_t flashsize;
    uint16_t id = 0;
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
  MX_SPI2_Init();
  /* USER CODE BEGIN 2 */
  lcd_init();                                   	/* 初始化LCD */
  norflash_init();                    				/* 初始化NORFLASH */

  lcd_show_string(30,  50, 200, 16, 16, "STM32", RED);
  lcd_show_string(30,  70, 200, 16, 16, "SPI TEST", RED);
  lcd_show_string(30,  90, 200, 16, 16, "ATOM@ALIENTEK", RED);
  lcd_show_string(30, 110, 200, 16, 16, "KEY1:Write  KEY0:Read", RED); /* 显示提示信息 */

  id = norflash_read_id(); /* 读取FLASH ID */

  while ((id == 0) || (id == 0XFFFF)) /* 检测不到FLASH芯片 */
  {
	  lcd_show_string(30, 130, 200, 16, 16, "FLASH Check Failed!", RED);
	  delay_ms(500);
	  lcd_show_string(30, 130, 200, 16, 16, "Please Check!      ", RED);
	  delay_ms(500);
	  LED0_TOGGLE(); /* LED0闪烁 */
  }

  lcd_show_string(30, 130, 200, 16, 16, "SPI FLASH Ready!", BLUE);
  flashsize = 16 * 1024 * 1024; /* FLASH 大小为16M字节 */
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  key = key_scan(0);

	  if (key == KEY1_PRES) /* KEY1按下,写入 */
	  {
		  lcd_fill(0, 150, 239, 319, WHITE); /* 清除半屏 */
		  lcd_show_string(30, 150, 200, 16, 16, "Start Write FLASH....", BLUE);
		  printf("111:%d\r\n",i);
		  sprintf((char *)datatemp, "%s%d", (char *)g_text_buf, i);
		  norflash_write((uint8_t *)datatemp, flashsize - 100, TEXT_SIZE);      /* 从倒数第100个地址处开始,写入SIZE长度的数据 */
		  lcd_show_string(30, 150, 200, 16, 16, "FLASH Write Finished!", BLUE); /* 提示传送完成 */
	  }

	  if (key == KEY0_PRES) /* KEY0按下,读取字符串并显示 */
	  {
		  lcd_show_string(30, 150, 200, 16, 16, "Start Read FLASH... . ", BLUE);
		  norflash_read(datatemp, flashsize - 100, TEXT_SIZE);                   /* 从倒数第100个地址处开始,读出SIZE个字节 */
		  lcd_show_string(30, 150, 200, 16, 16, "The Data Readed Is:   ", BLUE); /* 提示传送完成 */
		  lcd_show_string(30, 170, 200, 16, 16, (char *)datatemp, BLUE);         /* 显示读到的字符串 */
	  }

	  i++;

	  if (i == 20)
	  {
		  LED0_TOGGLE(); /* LED0闪烁 */
		  i = 0;
	  }

	  delay_ms(10);

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
