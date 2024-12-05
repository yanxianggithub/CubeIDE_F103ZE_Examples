/* USER CODE BEGIN Header */

/* USER CODE END Header */
/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include "sdio.h"
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
#include "../../Middlewares/MALLOC/malloc.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
HAL_SD_CardInfoTypeDef g_sd_card_info_handle; /* SD����Ϣ�ṹ�� */
extern uint8_t g_sd_error;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

/**
 * @brief       ͨ�����ڴ�ӡSD�������Ϣ
 * @param       ��
 * @retval      ��
 */
void show_sdcard_info(void)
{
    HAL_SD_CardCIDTypeDef sd_card_cid;

    HAL_SD_GetCardCID(&hsd, &sd_card_cid); 			/* ��ȡCID */
    get_sd_card_info(&g_sd_card_info_handle);   	/* ��ȡSD����Ϣ */

    switch (g_sd_card_info_handle.CardType)
    {
        case CARD_SDSC:
        {
            if (g_sd_card_info_handle.CardVersion == CARD_V1_X)
            {
                printf("Card Type:SDSC V1\r\n");
            }
            else if (g_sd_card_info_handle.CardVersion == CARD_V2_X)
            {
                printf("Card Type:SDSC V2\r\n");
            }
        }
        break;

        case CARD_SDHC_SDXC:
            printf("Card Type:SDHC\r\n");
            break;

        default: break;
    }

    printf("Card ManufacturerID:%d\r\n", sd_card_cid.ManufacturerID);                                               /* ������ID */
    printf("Card RCA:%lu\r\n", g_sd_card_info_handle.RelCardAdd);                                                  	/* ����Ե�ַ */
    printf("Card Capacity:%lu MB\r\n", (uint32_t)SD_TOTAL_SIZE_MB(&hsd));                               			/* ��ʾ���� */
    printf("Card BlockSize:%lu\r\n\r\n", g_sd_card_info_handle.BlockSize);                                      	/* ��ʾ���С */
}

/**
 * @brief       ����SD���Ķ�ȡ
 *   @note      ��secaddr��ַ��ʼ,��ȡseccnt������������
 * @param       secaddr : ������ַ
 * @param       seccnt  : ������
 * @retval      ��
 */
void sd_test_read(uint32_t secaddr, uint32_t seccnt)
{
    uint32_t i;
    uint8_t *buf;
    uint8_t sta = 0;

    buf = mymalloc(SRAMIN, seccnt * 512);     /* �����ڴ�,��SRAM�����ڴ� */
    sta = sd_read_disk(buf, secaddr, seccnt); /* ��ȡsecaddr������ʼ������ */

    if (sta == 0)
    {
        lcd_show_string(30, 170, 200, 16, 16, "USART1 Sending Data...", BLUE);
        printf("SECTOR %lu DATA:\r\n", secaddr);

        for (i = 0; i < seccnt * 512; i++)
        {
            printf("%x ", buf[i]); /* ��ӡsecaddr��ʼ���������� */
        }

        printf("\r\nDATA ENDED\r\n");
        lcd_show_string(30, 170, 200, 16, 16, "USART1 Send Data Over!", BLUE);
    }
    else
    {
        printf("err:%d\r\n", sta);
        lcd_show_string(30, 170, 200, 16, 16, "SD read Failure!      ", BLUE);
    }

    myfree(SRAMIN, buf); /* �ͷ��ڴ� */
}

/**
 * @brief       ����SD����д��
 *   @note      ��secaddr��ַ��ʼ,д��seccnt������������
 *              ����!! ���дȫ��0XFF������,���������SD��.
 *
 * @param       secaddr : ������ַ
 * @param       seccnt  : ������
 * @retval      ��
 */
void sd_test_write(uint32_t secaddr, uint32_t seccnt)
{
    uint32_t i;
    uint8_t *buf;
    uint8_t sta = 0;
    buf = mymalloc(SRAMIN, seccnt * 512);   /* ��SRAM�����ڴ� */

    for (i = 0; i < seccnt * 512; i++)      /* ��ʼ��д�������,��3�ı���. */
    {
        buf[i] = i * 3;
    }

    sta = sd_write_disk(buf, secaddr, seccnt); /* ��secaddr������ʼд��seccnt���������� */

    if (sta == 0)
    {
        printf("Write over!\r\n");
    }
    else
    {
        printf("err:%d\r\n", sta);
    }

    myfree(SRAMIN, buf); /* �ͷ��ڴ� */
}
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
    uint8_t t = 0;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  delay_init(72);						   	/* ��ʼ����ʱ���� */
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_FSMC_Init();
  MX_USART1_UART_Init();
  MX_SDIO_SD_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */
  lcd_init();                         		/* ��ʼ��LCD */
  my_mem_init(SRAMIN);               		/* ��ʼ���ڲ�SRAM�ڴ�� */

  lcd_show_string(30,  50, 200, 16, 16, "STM32", RED);
  lcd_show_string(30,  70, 200, 16, 16, "SD TEST", RED);
  lcd_show_string(30,  90, 200, 16, 16, "ATOM@ALIENTEK", RED);
  lcd_show_string(30, 110, 200, 16, 16, "KEY0:Read Sector 0", RED);
  while (g_sd_error == 1) /* ��ⲻ��SD�� */
  {
	  g_sd_error = 0;
	  MX_SDIO_SD_Init();
      lcd_show_string(30, 130, 200, 16, 16, "SD Card Error!", RED);
      delay_ms(500);
      lcd_show_string(30, 130, 200, 16, 16, "Please Check! ", RED);
      delay_ms(500);
      LED0_TOGGLE(); /* �����˸ */
  }
  /* ��ӡSD�������Ϣ */
  show_sdcard_info();

  /* ���SD���ɹ� */
  lcd_show_string(30, 130, 200, 16, 16, "SD Card OK    ", BLUE);
  lcd_show_string(30, 150, 200, 16, 16, "SD Card Size:     MB", BLUE);
  lcd_show_num(30 + 13 * 8, 150, SD_TOTAL_SIZE_MB(&hsd), 5, 16, BLUE); /* ��ʾSD������ */
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
      key = key_scan(0);

      if (key == KEY0_PRES)   	/* KEY0������ */
      {
          sd_test_read(0, 1);  	/* ������0��ȡ1*512�ֽڵ����� */
      }else if(key == KEY1_PRES)
      {
    	  sd_test_write(0, 1);	/* д���ݵ�����0 */
      }

      t++;
      delay_ms(10);

      if (t == 20)
      {
          LED0_TOGGLE(); 	 	/* �����˸ */
          t = 0;
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
