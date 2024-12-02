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
#include "../../BSP/NRF24L01/nrf24l01.h"
#include "../../SYSTEM/delay/delay.h"
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
    uint8_t key, mode;
    uint16_t t = 0;
    uint8_t tmp_buf[33];
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
  lcd_init();                                         /* 初始化LCD */
  nrf24l01_init();                    /* 初始化NRF24L01 */

  lcd_show_string(30, 50, 200, 16, 16, "STM32", RED);
  lcd_show_string(30, 70, 200, 16, 16, "NRF24L01 TEST", RED);
  lcd_show_string(30, 90, 200, 16, 16, "ATOM@ALIENTEK", RED);
  while (nrf24l01_check()) /* 检查NRF24L01是否在线 */
  {
      lcd_show_string(30, 110, 200, 16, 16, "NRF24L01 Error", RED);
      delay_ms(200);
      lcd_fill(30, 110, 239, 130 + 16, WHITE);
      delay_ms(200);
  }

  lcd_show_string(30, 110, 200, 16, 16, "NRF24L01 OK", RED);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  key = key_scan(0);

	  if (key == KEY0_PRES)
	  {
		  mode = 0; /* 接收模式 */
		  break;
	  }
	  else if (key == KEY1_PRES)
	  {
		  mode = 1; /* 发送模式 */
		  break;
	  }

	  t++;

	  if (t == 100) /* 显示提示信息 */
	  {
		  lcd_show_string(10, 130, 230, 16, 16, "KEY0:RX_Mode  KEY1:TX_Mode", RED);
	  }

	  if (t == 200) /* 关闭提示信息 */
	  {
		  lcd_fill(10, 130, 230, 150 + 16, WHITE);
		  t = 0;
	  }

	  delay_ms(5);
  }

  lcd_fill(10, 130, 240, 166, WHITE); /* 清空上面的显示 */

  if (mode == 0) /* RX模式 */
  {
	  lcd_show_string(30, 130, 200, 16, 16, "NRF24L01 RX_Mode", BLUE);
	  lcd_show_string(30, 150, 200, 16, 16, "Received DATA:", BLUE);
	  nrf24l01_rx_mode(); /* 进入RX模式 */

	  while (1)
	  {
		  if (nrf24l01_rx_packet(tmp_buf) == 0) /* 一旦接收到信息,则显示出来. */
		  {
			  tmp_buf[32] = 0; /* 加入字符串结束符 */
			  lcd_show_string(0, 170, lcddev.width - 1, 32, 16, (char *)tmp_buf, BLUE);
		  }
		  else
			  delay_us(100);

		  t++;

		  if (t == 10000) /* 大约1s钟改变一次状态 */
		  {
			  t = 0;
			  LED0_TOGGLE();
		  }
	  }
  }
  else /* TX模式 */
  {
	  lcd_show_string(30, 130, 200, 16, 16, "NRF24L01 TX_Mode", BLUE);
	  nrf24l01_tx_mode(); /* 进入TX模式 */
	  mode = ' ';         /* 从空格键开始发送 */

	  while (1)
	  {
		  if (nrf24l01_tx_packet(tmp_buf) == 0) /* 发送成功 */
		  {
			  lcd_show_string(30, 150, 239, 32, 16, "Sended DATA:", BLUE);
			  lcd_show_string(0, 170, lcddev.width - 1, 32, 16, (char *)tmp_buf, BLUE);
			  key = mode;

			  for (t = 0; t < 32; t++)
			  {
				  key++;

				  if (key > ('~'))
					  key = ' ';

				  tmp_buf[t] = key;
			  }

			  mode++;

			  if (mode > '~')
				  mode = ' ';

			  tmp_buf[32] = 0; /* 加入结束符 */
		  }
		  else
		  {
			  lcd_fill(0, 150, lcddev.width, 170 + 16 * 3, WHITE); /* 清空显示 */
			  lcd_show_string(30, 150, lcddev.width - 1, 32, 16, "Send Failed ", BLUE);
		  }

		  LED0_TOGGLE();
		  delay_ms(200);
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
