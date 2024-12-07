/* USER CODE BEGIN Header */
/**
 ****************************************************************************************************
 * @file        main.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2024-01-01
 * @brief       ����IAP ʵ��-IAP Bootloader V1.0
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
    uint32_t oldcount = 0;      /* �ϵĴ��ڽ�������ֵ */
    uint32_t applenth = 0;      /* ���յ���app���볤�� */
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
  delay_init(72);						   /* ��ʼ����ʱ���� */
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_FSMC_Init();
  MX_USART1_UART_Init();
  /* USER CODE BEGIN 2 */
  lcd_init();                                         /* ��ʼ��LCD */
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
		  if (oldcount == g_usart_rx_cnt)   /* ��������,û���յ��κ�����,��Ϊ�������ݽ������ */
		  {
			  applenth = g_usart_rx_cnt;
			  oldcount = 0;
			  g_usart_rx_cnt = 0;
			  printf("�û�����������!\r\n");
			  printf("���볤��:%luBytes\r\n", applenth);
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
				  lcd_fill(30, 190, 240, 210 + 16, WHITE);    /* �����ʾ */
			  }
		  }
	  }

	  key = key_scan(0);

	  if (key == WKUP_PRES)   /* WKUP����,���¹̼���FLASH */
	  {
		  if (applenth)
		  {
			  printf("��ʼ���¹̼�...\r\n");
			  lcd_show_string(30, 190, 200, 16, 16, "Copying APP2FLASH...", BLUE);

			  if (((*(volatile uint32_t *)(0X20002000 + 4)) & 0xFF000000) == 0x08000000)  /* �ж��Ƿ�Ϊ0X08XXXXXX */
			  {
				  iap_write_appbin(FLASH_APP1_ADDR, g_usart_rx_buf, applenth);            /* ����FLASH���� */
				  lcd_show_string(30, 190, 200, 16, 16, "Copy APP Successed!!", BLUE);
				  printf("�̼��������!\r\n");
			  }
			  else
			  {
				  lcd_show_string(30, 190, 200, 16, 16, "Illegal FLASH APP!  ", BLUE);
				  printf("��FLASHӦ�ó���!\r\n");
			  }
		  }
		  else
		  {
			  printf("û�п��Ը��µĹ̼�!\r\n");
			  lcd_show_string(30, 190, 200, 16, 16, "No APP!", BLUE);
		  }

		  clearflag = 7; /* ��־��������ʾ,��������7*300ms�������ʾ */
	  }

	  if (key == KEY1_PRES)   /* KEY1��������, ����FLASH APP���� */
	  {
		  if (((*(volatile uint32_t *)(FLASH_APP1_ADDR + 4)) & 0xFF000000) == 0x08000000) /* �ж�FLASH�����Ƿ���APP,�еĻ�ִ�� */
		  {
			  printf("��ʼִ��FLASH�û�����!!\r\n\r\n");
			  delay_ms(10);
			  iap_load_app(FLASH_APP1_ADDR);/* ִ��FLASH APP���� */

		  }
		  else
		  {
			  printf("û�п������еĹ̼�!\r\n");
			  lcd_show_string(30, 190, 200, 16, 16, "No APP!", BLUE);
		  }

		  clearflag = 7; /* ��־��������ʾ,��������7*300ms�������ʾ */
	  }

	  if (key == KEY0_PRES)   /* KEY0���� */
	  {
		  printf("��ʼִ��SRAM�û�����!!\r\n\r\n");
		  delay_ms(10);

		  if (((*(volatile uint32_t *)(0X20002000 + 4)) & 0xFF000000) == 0x20000000)   /* �ж��Ƿ�Ϊ0X20XXXXXX */
		  {
			  iap_load_app(0X20002000);   /* SRAM��ַ */
		  }
		  else
		  {
			  printf("��SRAMӦ�ó���,�޷�ִ��!\r\n");
			  lcd_show_string(30, 190, 200, 16, 16, "Illegal SRAM APP!", BLUE);
		  }

		  clearflag = 7; /* ��־��������ʾ,��������7*300ms�������ʾ */
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
