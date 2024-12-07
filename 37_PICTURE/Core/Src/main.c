/* USER CODE BEGIN Header */
/**
 ****************************************************************************************************
 * @file        main.c
 * @author      ����ԭ���Ŷ�(ALIENTEK)
 * @version     V1.0
 * @date        2024-01-01
 * @brief       ͼƬ��ʾ ʵ��
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
#include "../../ATK_Middlewares/PICTURE/piclib.h"
#include "../../FatFs/exfuns/exfuns.h"
#include "../../ATK_Middlewares/TEXT/text.h"
#include "ff.h"
#include "math.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
uint16_t pic_get_tnum(char *path);
/**
 * @brief       �õ�path·����,Ŀ���ļ����ܸ���
 * @param       path : ·��
 * @retval      ����Ч�ļ���
 */
uint16_t pic_get_tnum(char *path)
{
    uint8_t res;
    uint16_t rval = 0;
    DIR tdir;                                      	/* ��ʱĿ¼ */
	FILINFO tfileinfo;								/* ��ʱ�ļ���Ϣ */
	uint8_t *fn;
    res = f_opendir(&tdir, (const TCHAR*)path); 	/* ��Ŀ¼ */
  	tfileinfo.lfsize = _MAX_LFN * 2 + 1;			/* ���ļ�����󳤶� */
	tfileinfo.lfname = mymalloc(SRAMIN, tfileinfo.lfsize);/* Ϊ���ļ������������ڴ� */
	if(res == FR_OK && tfileinfo.lfname != NULL)
	{
		while(1)//��ѯ�ܵ���Ч�ļ���
		{
	        res = f_readdir(&tdir, &tfileinfo);       			/* ��ȡĿ¼�µ�һ���ļ� */
	        if(res != FR_OK || tfileinfo.fname[0] == 0)break;	/* ������/��ĩβ��,�˳� */
     		fn = (uint8_t*)(*tfileinfo.lfname? tfileinfo.lfname : tfileinfo.fname);
			res = exfuns_file_type((char*)fn);
			if((res & 0XF0) == 0X50)//ȡ����λ,�����ǲ���ͼƬ�ļ�
			{
				rval++;//��Ч�ļ�������1
			}
		}
	}
	return rval;
}

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
    uint8_t res;
    DIR picdir;           /* ͼƬĿ¼ */
    FILINFO picfileinfo;  /* �ļ���Ϣ */
    char *pname;          /* ��·�����ļ��� */
    uint8_t *fn;   		  /* ���ļ��� */
    uint16_t totpicnum;   /* ͼƬ�ļ����� */
    uint16_t curindex;    /* ͼƬ��ǰ���� */
    uint8_t key;          /* ��ֵ */
    uint8_t pause = 0;    /* ��ͣ��� */
    uint8_t t;
    uint16_t temp;
    uint32_t *picoffsettbl; /* ͼƬ�ļ�offset������ */
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
  MX_FATFS_Init();
  MX_SPI2_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */
  lcd_init();                         		/* ��ʼ��LCD */
  norflash_init();                    		/* ��ʼ��NORFLASH */
  my_mem_init(SRAMIN);               		/* ��ʼ���ڲ�SRAM�ڴ�� */
  exfuns_init();                 			/* Ϊfatfs��ر��������ڴ� */

  f_mount(fs[0], "0:", 1);       			/* ����SD�� */
  f_mount(fs[1], "1:", 1);       			/* ����SPI_FLASH�� */

  while (fonts_init())                		/* ����ֿ� */
  {
      lcd_show_string(30, 50, 200, 16, 16, "Font Error!", RED);
      delay_ms(200);
      lcd_fill(30, 50, 240, 66, WHITE); 	/* �����ʾ */
      delay_ms(200);
  }

  text_show_string(30,  50, 200, 16, "����ԭ��STM32������", 16, 0, RED);
  text_show_string(30,  70, 200, 16, "ͼƬ��ʾ ʵ��", 16, 0, RED);
  text_show_string(30,  90, 200, 16, "KEY0:NEXT KEY1:PREV", 16, 0, RED);
  text_show_string(30, 110, 200, 16, "KEY_UP:PAUSE", 16, 0, RED);
  text_show_string(30, 130, 200, 16, "����ԭ��@ALIENTEK", 16, 0, RED);

  while (f_opendir(&picdir, "0:/PICTURE")) /* ��ͼƬ�ļ��� */
  {
	  text_show_string(30, 150, 240, 16, "PICTURE�ļ��д���!", 16, 0, RED);
	  delay_ms(200);
	  lcd_fill(30, 150, 240, 186, WHITE); 	/* �����ʾ */
	  delay_ms(200);
  }

  totpicnum = pic_get_tnum("0:/PICTURE"); 	/* �õ�����Ч�ļ��� */

  while (totpicnum == NULL) 				/* ͼƬ�ļ�Ϊ0 */
  {
	  text_show_string(30, 150, 240, 16, "û��ͼƬ�ļ�!", 16, 0, RED);
	  delay_ms(200);
	  lcd_fill(30, 150, 240, 186, WHITE); 	/* �����ʾ */
	  delay_ms(200);
  }
  picfileinfo.lfsize = _MAX_LFN * 2 + 1;						/* ���ļ�����󳤶� */
  picfileinfo.lfname = mymalloc(SRAMIN, picfileinfo.lfsize);	/* Ϊ���ļ������������ڴ� */
  pname=mymalloc(SRAMIN, picfileinfo.lfsize);					/* Ϊ��·�����ļ��������ڴ� */
  picoffsettbl = mymalloc(SRAMIN, 2 * totpicnum);             	/* ����4*totpicnum���ֽڵ��ڴ�,���ڴ��ͼƬ���� */

  while (!picfileinfo.lfname || !pname || !picoffsettbl) 		/* �ڴ������� */
  {
	  text_show_string(30, 150, 240, 16, "�ڴ����ʧ��!", 16, 0, RED);
	  delay_ms(200);
	  lcd_fill(30, 150, 240, 186, WHITE); /* �����ʾ */
	  delay_ms(200);
  }

  /* ��¼���� */
  res = f_opendir(&picdir, "0:/PICTURE"); /* ��Ŀ¼ */

  if (res == FR_OK)
  {
	  curindex = 0; /* ��ǰ����Ϊ0 */

	  while (1) /* ȫ����ѯһ�� */
	  {
		  temp = picdir.index;                    /* ��¼��ǰdptrƫ�� */
		  res = f_readdir(&picdir, &picfileinfo); /* ��ȡĿ¼�µ�һ���ļ� */

		  if (res != FR_OK || picfileinfo.fname[0] == 0)
			  break; /* ������/��ĩβ��,�˳� */
		  fn = (uint8_t*)(*picfileinfo.lfname? picfileinfo.lfname : picfileinfo.fname);
		  res = exfuns_file_type((char *)fn);

		  if ((res & 0XF0) == 0X50) 			/* ȡ����λ,�����ǲ���ͼƬ�ļ� */
		  {
			  picoffsettbl[curindex] = temp; 	/* ��¼���� */
			  curindex++;
		  }
	  }
  }

  text_show_string(30, 150, 240, 16, "��ʼ��ʾ...", 16, 0, RED);
  delay_ms(1500);
  piclib_init();                                         /* ��ʼ����ͼ */
  curindex = 0;                                          /* ��0��ʼ��ʾ */
  res = f_opendir(&picdir, (const TCHAR *)"0:/PICTURE"); /* ��Ŀ¼ */

  while (res == FR_OK) /* �򿪳ɹ� */
  {
	  dir_sdi(&picdir, picoffsettbl[curindex]); 		/* �ı䵱ǰĿ¼���� */
	  res = f_readdir(&picdir, &picfileinfo);    		/* ��ȡĿ¼�µ�һ���ļ� */

	  if (res != FR_OK || picfileinfo.fname[0] == 0)
		  break; /* ������/��ĩβ��,�˳� */
	  fn = (uint8_t*)(*picfileinfo.lfname? picfileinfo.lfname : picfileinfo.fname);
	  strcpy((char *)pname, "0:/PICTURE/");          	/* ����·��(Ŀ¼) */
	  strcat((char *)pname, (const char *)fn); 			/* ���ļ������ں��� */
	  lcd_clear(BLACK);
	  piclib_ai_load_picfile(pname, 0, 0, lcddev.width, lcddev.height, 1); /* ��ʾͼƬ */
	  text_show_string(2, 2, lcddev.width, 16, (char *)pname, 16, 1, RED); /* ��ʾͼƬ���� */
	  t = 0;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
      key = key_scan(0); /* ɨ�谴�� */

      if (t > 250)
          key = 1; /* ģ��һ�ΰ���KEY0 */

      if ((t % 20) == 0)
      {
          LED0_TOGGLE(); /* LED0��˸,��ʾ������������. */
      }

      if (key == KEY1_PRES) /* ��һ�� */
      {
          if (curindex)
          {
              curindex--;
          }
          else
          {
              curindex = totpicnum - 1;
          }

          break;
      }
      else if (key == KEY0_PRES) /* ��һ�� */
      {
          curindex++;

          if (curindex >= totpicnum)
              curindex = 0; /* ��ĩβ��ʱ��,�Զ���ͷ��ʼ */

          break;
      }
      else if (key == WKUP_PRES)
      {
          pause = !pause;
          LED1(!pause); /* ��ͣ��ʱ��LED1��. */
      }

      if (pause == 0)
          t++;

      delay_ms(10);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  res = 0;
  }
  myfree(SRAMIN, picfileinfo.lfname);  	/* �ͷ��ڴ� */
  myfree(SRAMIN, pname);        		/* �ͷ��ڴ� */
  myfree(SRAMIN, picoffsettbl); 		/* �ͷ��ڴ� */
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
