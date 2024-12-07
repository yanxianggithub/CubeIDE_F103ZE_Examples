/* USER CODE BEGIN Header */
/**
 ****************************************************************************************************
 * @file        main.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2024-01-01
 * @brief       DSP BasicMath ʵ��
 * @license     Copyright (c) 2020-2032, ������������ӿƼ����޹�˾
 ****************************************************************************************************
 * @attention
 *
 * ʵ��ƽ̨:����ԭ�� STM32F103������
 * ������Ƶ:www.yuanzige.com
 * ������̳:www.openedv.com
 * ��˾��ַ:www.alientek.com
 * �����ַ:openedv.taobao.com
 *
 ****************************************************************************************************
 */
/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "tim.h"
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
#include "arm_math.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
#define DELTA   0.0001f                 /* ���ֵ */
uint8_t g_timeout;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/**
 * @brief       sin cos ����
 * @param       angle : ��ʼ�Ƕ�
 * @param       times : �������
 * @param       mode  : �Ƿ�ʹ��DSP��
 *   @arg       0 , ��ʹ��DSP��;
 *   @arg       1 , ʹ��DSP��;
 *
 * @retval      ��
 */
uint8_t sin_cos_test(float angle, uint32_t times, uint8_t mode)
{
    float sinx, cosx;
    float result;
    uint32_t i = 0;

    if (mode == 0)
    {
        for (i = 0; i < times; i++)
        {
            cosx = cosf(angle);                 /* ��ʹ��DSP�Ż���sin��cos���� */
            sinx = sinf(angle);
            result = sinx * sinx + cosx * cosx; /* ������Ӧ�õ���1 */
            result = fabsf(result - 1.0f);      /* �Ա���1�Ĳ�ֵ */

            if (result > DELTA)return 0XFF;     /* �ж�ʧ�� */

            angle += 0.001f;    /* �Ƕ����� */
        }
    }
    else
    {
        for (i = 0; i < times; i++)
        {

            cosx = arm_cos_f32(angle);          /* ʹ��DSP�Ż���sin��cos���� */
            sinx = arm_sin_f32(angle);
            result = sinx * sinx + cosx * cosx; /* ������Ӧ�õ���1 */
            result = fabsf(result - 1.0f);      /* �Ա���1�Ĳ�ֵ */

            if (result > DELTA)return 0XFF;     /* �ж�ʧ�� */

            angle += 0.001f;    /* �Ƕ����� */
        }
    }

    return 0;   /* ������� */
}
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
    float time;
    char buf[50];
    uint8_t res;
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
  MX_TIM6_Init();
  /* USER CODE BEGIN 2 */
  lcd_init();                                         /* ��ʼ��LCD */
  lcd_show_string(30,  50, 200, 16, 16, "STM32", RED);
  lcd_show_string(30,  70, 200, 16, 16, "DSP BasicMath TEST", RED);
  lcd_show_string(30,  90, 200, 16, 16, "ATOM@ALIENTEK", RED);
  lcd_show_string(30, 110, 200, 16, 16, " No DSP runtime:", RED); /* ��ʾ��ʾ��Ϣ */
  lcd_show_string(30, 150, 200, 16, 16, "Use DSP runtime:", RED); /* ��ʾ��ʾ��Ϣ */
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  /* ��ʹ��DSP�Ż� */
	  __HAL_TIM_SET_COUNTER(&htim6,0);/* ����TIM6��ʱ���ļ�����ֵ */
	  g_timeout = 0;
	  res = sin_cos_test(PI / 6, 10000, 0);
	  time = __HAL_TIM_GET_COUNTER(&htim6) + (uint32_t)g_timeout * 65536;
	  sprintf(buf, "%0.1fms\r\n", time / 10);

	  if (res == 0)
	  {
		  lcd_show_string(30 + 16 * 8, 110, 100, 16, 16, buf, BLUE);      /* ��ʾ����ʱ�� */
	  }
	  else
	  {
		  lcd_show_string(30 + 16 * 8, 110, 100, 16, 16, "error��", BLUE); /* ��ʾ��ǰ������� */
	  }

	  /* ʹ��DSP�Ż� */
	  __HAL_TIM_SET_COUNTER(&htim6,0); /* ����TIM6��ʱ���ļ�����ֵ */
	  g_timeout = 0;
	  res = sin_cos_test(PI / 6, 10000, 1);
	  time = __HAL_TIM_GET_COUNTER(&htim6) + (uint32_t)g_timeout * 65536;
	  sprintf(buf, "%0.1fms\r\n", time / 10);

	  if (res == 0)
	  {
		  lcd_show_string(30 + 16 * 8, 150, 100, 16, 16, buf, BLUE);      /* ��ʾ����ʱ�� */
	  }
	  else
	  {
		  lcd_show_string(30 + 16 * 8, 150, 100, 16, 16, "error��", BLUE); /* ��ʾ���� */
	  }

	  LED0_TOGGLE();

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
