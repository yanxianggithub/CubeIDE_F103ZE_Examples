/* USER CODE BEGIN Header */

/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "adc.h"
#include "dac.h"
#include "dma.h"
#include "tim.h"
#include "usart.h"
#include "gpio.h"
#include "fsmc.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "../../BSP/LED/led.h"
#include "../../BSP/KEY/key.h"
#include "../../BSP/LCD/lcd.h"
#include "../../SYSTEM/delay/delay.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
extern uint16_t g_dac_sin_buf[4096];           /* �������ݻ����� */
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
    uint16_t adcx;
    float temp;
    uint8_t t = 0;
    uint8_t key;
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
  MX_DMA_Init();
  MX_FSMC_Init();
  MX_USART1_UART_Init();
  MX_DAC_Init();
  MX_ADC3_Init();
  MX_TIM7_Init();
  /* USER CODE BEGIN 2 */
  lcd_init();    			/* ��ʼ��LCD */
  adc3_channel_set(&hadc3, ADC_CHANNEL_1, ADC_REGULAR_RANK_1, ADC_SAMPLETIME_1CYCLE_5);
  HAL_DMA_Start(&hdma_dac_ch1, (uint32_t)&DAC1->DHR12R1, (uint32_t)g_dac_sin_buf, 0);                          /* ����DMA������� */

  lcd_show_string(30,  50, 200, 16, 16, "STM32", RED);
  lcd_show_string(30,  70, 200, 16, 16, "DAC DMA Sine WAVE TEST", RED);
  lcd_show_string(30,  90, 200, 16, 16, "ATOM@ALIENTEK", RED);
  lcd_show_string(30, 110, 200, 16, 16, "KEY0:3Khz  KEY1:30Khz", RED);

  lcd_show_string(30, 130, 200, 16, 16, "DAC VAL:", BLUE);
  lcd_show_string(30, 150, 200, 16, 16, "DAC VOL:0.000V", BLUE);
  lcd_show_string(30, 170, 200, 16, 16, "ADC VOL:0.000V", BLUE);
  dac_creat_sin_buf(2048, 100);

  dac_dma_wave_enable(100, 10 - 1, 72 - 1);               /* 100Khz����Ƶ��, 100����, �õ�1Khz�����Ҳ� */

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  t++;
	  key = key_scan(0);                                  /* ����ɨ�� */

	  if (key == KEY0_PRES)                               /* �߲����� , Լ1Khz���� */
	  {
		  dac_creat_sin_buf(2048, 100);
		  dac_dma_wave_enable(100, 10 - 1, 24 - 1);       /* 300Khz����Ƶ��, 100����, �õ����3KHz�����Ҳ�. */
	  }
	  else if (key == KEY1_PRES)                          /* �Ͳ����� , Լ1Khz���� */
	  {
		  dac_creat_sin_buf(2048, 10);
		  dac_dma_wave_enable(10, 10 - 1, 24 - 1);        /* 300Khz����Ƶ��, 10����, ���Եõ����30KHz�����Ҳ�. */
	  }

	  adcx = DAC1->DHR12R1;                               /* ��ȡDAC1_OUT1�����״̬ */
	  lcd_show_xnum(94, 130, adcx, 4, 16, 0, BLUE);       /* ��ʾDAC�Ĵ���ֵ */

	  temp = (float)adcx * (3.3 / 4096);                  /* �õ�DAC��ѹֵ */
	  adcx = temp;
	  lcd_show_xnum(94, 150, temp, 1, 16, 0, BLUE);       /* ��ʾ��ѹֵ�������� */

	  temp -= adcx;
	  temp *= 1000;
	  lcd_show_xnum(110, 150, temp, 3, 16, 0X80, BLUE);   /* ��ʾ��ѹֵ��С������ */

	  adcx = adc3_get_result_average(ADC_CHANNEL_1, 10);  /* �õ�ADC3ͨ��1��ת����� */
	  temp = (float)adcx * (3.3 / 4096);                  /* �õ�ADC��ѹֵ(adc��12bit��) */
	  adcx = temp;
	  lcd_show_xnum(94, 170, temp, 1, 16, 0, BLUE);       /* ��ʾ��ѹֵ�������� */

	  temp -= adcx;
	  temp *= 1000;
	  lcd_show_xnum(110, 170, temp, 3, 16, 0X80, BLUE);   /* ��ʾ��ѹֵ��С������ */

	  if (t == 40)        /* ��ʱʱ�䵽�� */
	  {
		  LED0_TOGGLE();  /* LED0��˸ */
		  t = 0;
	  }

	  delay_ms(5);

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
  PeriphClkInit.PeriphClockSelection = RCC_PERIPHCLK_ADC;
  PeriphClkInit.AdcClockSelection = RCC_ADCPCLK2_DIV6;
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
