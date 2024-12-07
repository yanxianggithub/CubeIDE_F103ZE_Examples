/* USER CODE BEGIN Header */
/**
 ****************************************************************************************************
 * @file        main.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2024-01-01
 * @brief       DSP BasicMath 实验
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
#define DELTA   0.0001f                 /* 误差值 */
uint8_t g_timeout;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/**
 * @brief       sin cos 测试
 * @param       angle : 起始角度
 * @param       times : 运算次数
 * @param       mode  : 是否使用DSP库
 *   @arg       0 , 不使用DSP库;
 *   @arg       1 , 使用DSP库;
 *
 * @retval      无
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
            cosx = cosf(angle);                 /* 不使用DSP优化的sin，cos函数 */
            sinx = sinf(angle);
            result = sinx * sinx + cosx * cosx; /* 计算结果应该等于1 */
            result = fabsf(result - 1.0f);      /* 对比与1的差值 */

            if (result > DELTA)return 0XFF;     /* 判断失败 */

            angle += 0.001f;    /* 角度自增 */
        }
    }
    else
    {
        for (i = 0; i < times; i++)
        {

            cosx = arm_cos_f32(angle);          /* 使用DSP优化的sin，cos函数 */
            sinx = arm_sin_f32(angle);
            result = sinx * sinx + cosx * cosx; /* 计算结果应该等于1 */
            result = fabsf(result - 1.0f);      /* 对比与1的差值 */

            if (result > DELTA)return 0XFF;     /* 判断失败 */

            angle += 0.001f;    /* 角度自增 */
        }
    }

    return 0;   /* 任务完成 */
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
  delay_init(72);						   /* 初始化延时函数 */
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_FSMC_Init();
  MX_USART1_UART_Init();
  MX_TIM6_Init();
  /* USER CODE BEGIN 2 */
  lcd_init();                                         /* 初始化LCD */
  lcd_show_string(30,  50, 200, 16, 16, "STM32", RED);
  lcd_show_string(30,  70, 200, 16, 16, "DSP BasicMath TEST", RED);
  lcd_show_string(30,  90, 200, 16, 16, "ATOM@ALIENTEK", RED);
  lcd_show_string(30, 110, 200, 16, 16, " No DSP runtime:", RED); /* 显示提示信息 */
  lcd_show_string(30, 150, 200, 16, 16, "Use DSP runtime:", RED); /* 显示提示信息 */
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  /* 不使用DSP优化 */
	  __HAL_TIM_SET_COUNTER(&htim6,0);/* 重设TIM6定时器的计数器值 */
	  g_timeout = 0;
	  res = sin_cos_test(PI / 6, 10000, 0);
	  time = __HAL_TIM_GET_COUNTER(&htim6) + (uint32_t)g_timeout * 65536;
	  sprintf(buf, "%0.1fms\r\n", time / 10);

	  if (res == 0)
	  {
		  lcd_show_string(30 + 16 * 8, 110, 100, 16, 16, buf, BLUE);      /* 显示运行时间 */
	  }
	  else
	  {
		  lcd_show_string(30 + 16 * 8, 110, 100, 16, 16, "error！", BLUE); /* 显示当前运行情况 */
	  }

	  /* 使用DSP优化 */
	  __HAL_TIM_SET_COUNTER(&htim6,0); /* 重设TIM6定时器的计数器值 */
	  g_timeout = 0;
	  res = sin_cos_test(PI / 6, 10000, 1);
	  time = __HAL_TIM_GET_COUNTER(&htim6) + (uint32_t)g_timeout * 65536;
	  sprintf(buf, "%0.1fms\r\n", time / 10);

	  if (res == 0)
	  {
		  lcd_show_string(30 + 16 * 8, 150, 100, 16, 16, buf, BLUE);      /* 显示运行时间 */
	  }
	  else
	  {
		  lcd_show_string(30 + 16 * 8, 150, 100, 16, 16, "error！", BLUE); /* 显示错误 */
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
