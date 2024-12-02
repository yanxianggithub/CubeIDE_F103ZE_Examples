/* USER CODE BEGIN Header */

/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "can.h"
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
    uint8_t key;
    uint8_t i = 0, t = 0;
    uint8_t cnt = 0;
    uint8_t canbuf[8];
    uint8_t rxlen = 0;
    uint8_t res;
    uint8_t mode = 1; /* CAN����ģʽ: 0,����ģʽ; 1,�ػ�ģʽ */
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  delay_init(72);						   /* ��ʼ����ʱ���� */
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_FSMC_Init();
  MX_USART1_UART_Init();
  MX_CAN_Init();
  /* USER CODE BEGIN 2 */
  lcd_init();                                         /* ��ʼ��LCD */
  lcd_show_string(30, 50, 200, 16, 16, "STM32", RED);
  lcd_show_string(30, 70, 200, 16, 16, "CAN TEST", RED);
  lcd_show_string(30, 90, 200, 16, 16, "ATOM@ALIENTEK", RED);
  lcd_show_string(30, 110, 200, 16, 16, "LoopBack Mode", RED);
  lcd_show_string(30, 130, 200, 16, 16, "KEY0:Send KEK_UP:Mode", RED); /* ��ʾ��ʾ��Ϣ */

  lcd_show_string(30, 150, 200, 16, 16, "Count:", RED);        /* ��ʾ��ǰ����ֵ */
  lcd_show_string(30, 170, 200, 16, 16, "Send Data:", RED);    /* ��ʾ���͵����� */
  lcd_show_string(30, 230, 200, 16, 16, "Receive Data:", RED); /* ��ʾ���յ������� */
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  key = key_scan(0);

	  if (key == KEY0_PRES) /* KEY0����,����һ������ */
	  {
		  for (i = 0; i < 8; i++)
		  {
			  canbuf[i] = cnt + i; /* ��䷢�ͻ����� */

			  if (i < 4)
			  {
				  lcd_show_xnum(30 + i * 32, 190, canbuf[i], 3, 16, 0X80, BLUE); /* ��ʾ���� */
			  }
			  else
			  {
				  lcd_show_xnum(30 + (i - 4) * 32, 210, canbuf[i], 3, 16, 0X80, BLUE); /* ��ʾ���� */
			  }
		  }

		  res = can_send_msg(0X12, canbuf, 8); /* ID = 0X12, ����8���ֽ� */

		  if (res)
		  {
			  lcd_show_string(30 + 80, 170, 200, 16, 16, "Failed", BLUE); /* ��ʾ����ʧ�� */
		  }
		  else
		  {
			  lcd_show_string(30 + 80, 170, 200, 16, 16, "OK    ", BLUE); /* ��ʾ���ͳɹ� */
		  }
	  }
	  else if (key == WKUP_PRES) /* WK_UP���£��ı�CAN�Ĺ���ģʽ */
	  {
		  mode = !mode;

		  if (mode == 0) /* ����ģʽ����Ҫ2�������� */
		  {
			  can_set_mode(CAN_MODE_NORMAL);		/* CAN����ģʽ��ʼ��, ����ģʽ, ������500Kbps */
			  lcd_show_string(30, 110, 200, 16, 16, "Nnormal Mode ", RED);
		  }
		  else /* �ػ�ģʽ,һ��������Ϳ��Բ�����. */
		  {
			  can_set_mode(CAN_MODE_LOOPBACK);		/* CAN�ػ�ģʽ��ʼ��, �ػ�ģʽ, ������500Kbps */
			  lcd_show_string(30, 110, 200, 16, 16, "LoopBack Mode", RED);
		  }
	  }

	  rxlen = can_receive_msg(0X12, canbuf); /* CAN ID = 0X12, �������ݲ�ѯ */

	  if (rxlen) /* ���յ������� */
	  {
		  lcd_fill(30, 270, 130, 310, WHITE); /* ���֮ǰ����ʾ */

		  for (i = 0; i < rxlen; i++)
		  {
			  if (i < 4)
			  {
				  lcd_show_xnum(30 + i * 32, 250, canbuf[i], 3, 16, 0X80, BLUE); /* ��ʾ���� */
			  }
			  else
			  {
				  lcd_show_xnum(30 + (i - 4) * 32, 270, canbuf[i], 3, 16, 0X80, BLUE); /* ��ʾ���� */
			  }
		  }
	  }

	  t++;
	  delay_ms(10);

	  if (t == 20)
	  {
		  LED0_TOGGLE(); /* ��ʾϵͳ�������� */
		  t = 0;
		  cnt++;
		  lcd_show_xnum(30 + 48, 150, cnt, 3, 16, 0X80, BLUE); /* ��ʾ���� */
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
