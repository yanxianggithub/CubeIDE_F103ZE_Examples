/* USER CODE BEGIN Header */
/**
 ****************************************************************************************************
 * @file        main.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2024-01-01
 * @brief       FATFS 实验
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
#include "../../FatFs/exfuns/exfuns.h"
#include "ff.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
extern uint8_t g_sd_sta;
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
uint8_t read_buf[50] = {0};
uint8_t write_buf[50] = "Hello test demo";

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
    uint32_t total, free;
    uint8_t t = 0, key = 0;
    UINT bw = 0, br = 0;
    uint8_t res = 0;
    FIL *f_fil;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  delay_init(72);						   	/* 初始化延时函数 */
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_FSMC_Init();
  MX_USART1_UART_Init();
  MX_SDIO_SD_Init();
//  MX_FATFS_Init();
  MX_SPI2_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */
  lcd_init();                         		/* 初始化LCD */
  norflash_init();                    		/* 初始化NORFLASH */
  my_mem_init(SRAMIN);               		/* 初始化内部SRAM内存池 */
  lcd_show_string(30,  50, 200, 16, 16, "STM32", RED);
  lcd_show_string(30,  70, 200, 16, 16, "FATFS TEST", RED);
  lcd_show_string(30,  90, 200, 16, 16, "ATOM@ALIENTEK", RED);
  lcd_show_string(30, 110, 200, 16, 16, "Use USMART for test", RED);
  while (g_sd_sta == 1) 					/* 检测不到SD卡 */
  {
	  g_sd_sta = 0;
	  MX_SDIO_SD_Init();
      lcd_show_string(30, 130, 200, 16, 16, "SD Card Error!", RED);
      delay_ms(500);
      lcd_show_string(30, 130, 200, 16, 16, "Please Check! ", RED);
      delay_ms(500);
      LED0_TOGGLE(); /* LED0闪烁 */
  }
  exfuns_init();                 		/* 为fatfs相关变量申请内存 */

  f_mount(fs[0], "0:", 1);       		/* 挂载SD卡 */
  res = f_mount(fs[1], "1:", 1);       	/* 挂载SPI_FLASH卡 */
  if (res == FR_NO_FILESYSTEM)			/* FLASH磁盘,FAT文件系统错误,重新格式化FLASH */
  {
	  lcd_show_string(30, 130, 200, 16, 16, "Flash Disk Formatting...", RED); /* 格式化FLASH */
	  res = f_mkfs("1:", 0, _MAX_SS);                                    	  /* 格式化FLASH,1:,盘符;0,使用默认格式化参数 */

	  if (res == 0)
	  {
		  f_setlabel((const TCHAR *)"1:ALIENTEK");                                /* 设置Flash磁盘的名字为：ALIENTEK */
		  lcd_show_string(30, 130, 200, 16, 16, "Flash Disk Format Finish", RED); /* 格式化完成 */
	  }
	  else
	  {
		  lcd_show_string(30, 130, 200, 16, 16, "Flash Disk Format Error ", RED); /* 格式化失败 */
	  }

	  delay_ms(1000);
  }

  lcd_fill(30, 130, 240, 150 + 16, WHITE);    /* 清除显示 */
  while (exfuns_get_free((uint8_t*)"0", &total, &free)) /* 得到SD卡的总容量和剩余容量 */
  {
      lcd_show_string(30, 130, 200, 16, 16, "SD Card Fatfs Error!", RED);
      delay_ms(200);
      lcd_fill(30, 130, 240, 150 + 16, WHITE); /* 清除显示 */
      delay_ms(200);
      LED0_TOGGLE(); /* LED0闪烁 */
  }
  lcd_show_string(30, 130, 200, 16, 16, "FATFS OK!", BLUE);
  lcd_show_string(30, 150, 200, 16, 16, "SD Total Size:     MB", BLUE);
  lcd_show_string(30, 170, 200, 16, 16, "SD  Free Size:     MB", BLUE);
  lcd_show_num(30 + 8 * 14, 150, total >> 10, 5, 16, BLUE);               /* 显示SD卡总容量 MB */
  lcd_show_num(30 + 8 * 14, 170, free >> 10, 5, 16, BLUE);                /* 显示SD卡剩余容量 MB */

  f_fil = (FIL *)mymalloc(SRAMIN, sizeof(f_fil));      		/* 为file申请内存 */
  if (f_fil == NULL)                                      	/* 内存申请失败 */
  {
	  myfree(SRAMIN, f_fil);
  }
  printf("文件系统挂载成功\r\n");
  printf("************ 即将进行文件读写测试 ************\r\n");

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  /*------------------- 文件系统测试：读写测试 ---------------------*/
	 key = key_scan(0);
	 if(key == KEY0_PRES)
	 {
		 res = f_open(f_fil, "1:test.txt", FA_OPEN_ALWAYS | FA_WRITE | FA_READ);/* 打开文件，没有文件就创建新文件 */
		 if(res == FR_OK)
		 {
			 f_lseek (f_fil, 0);			/* 偏移到初始位置写数据 */
			 res = f_write(f_fil, write_buf, sizeof(write_buf), &bw);
			 if(res == FR_OK)
			 {
				 printf("数据写入成功！！\r\n");
				 lcd_show_string(30, 190, 200, 16, 16, "write OK", BLUE);
			 }
		 }else
		 {
			 lcd_show_string(30, 190, 200, 16, 16, "read and write Error", BLUE);
		 }
	 }else if(key == KEY1_PRES)
	 {
		 f_lseek (f_fil, 0);				/* 偏移到初始位置读数据 */
		 res = f_read(f_fil, read_buf, sizeof(read_buf), &br);
		 lcd_show_string(30, 210, 200, 16, 16, (char *)read_buf, BLUE);
		 printf("读取到的数据位：%s\r\n",read_buf);
		 printf("读取字节数据：%d\r\n", br);
		 f_close(f_fil);
	 }

	  t++;
	  if(t % 20 == 0)
	  {
		  LED0_TOGGLE(); /* LED0闪烁 */
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
