/* USER CODE BEGIN Header */
/**
 ****************************************************************************************************
 * @file        main.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2024-01-01
 * @brief       图片显示 实验
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
 * @brief       得到path路径下,目标文件的总个数
 * @param       path : 路径
 * @retval      总有效文件数
 */
uint16_t pic_get_tnum(char *path)
{
    uint8_t res;
    uint16_t rval = 0;
    DIR tdir;                                      	/* 临时目录 */
	FILINFO tfileinfo;								/* 临时文件信息 */
	uint8_t *fn;
    res = f_opendir(&tdir, (const TCHAR*)path); 	/* 打开目录 */
  	tfileinfo.lfsize = _MAX_LFN * 2 + 1;			/* 长文件名最大长度 */
	tfileinfo.lfname = mymalloc(SRAMIN, tfileinfo.lfsize);/* 为长文件缓存区分配内存 */
	if(res == FR_OK && tfileinfo.lfname != NULL)
	{
		while(1)//查询总的有效文件数
		{
	        res = f_readdir(&tdir, &tfileinfo);       			/* 读取目录下的一个文件 */
	        if(res != FR_OK || tfileinfo.fname[0] == 0)break;	/* 错误了/到末尾了,退出 */
     		fn = (uint8_t*)(*tfileinfo.lfname? tfileinfo.lfname : tfileinfo.fname);
			res = exfuns_file_type((char*)fn);
			if((res & 0XF0) == 0X50)//取高四位,看看是不是图片文件
			{
				rval++;//有效文件数增加1
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
    DIR picdir;           /* 图片目录 */
    FILINFO picfileinfo;  /* 文件信息 */
    char *pname;          /* 带路径的文件名 */
    uint8_t *fn;   		  /* 长文件名 */
    uint16_t totpicnum;   /* 图片文件总数 */
    uint16_t curindex;    /* 图片当前索引 */
    uint8_t key;          /* 键值 */
    uint8_t pause = 0;    /* 暂停标记 */
    uint8_t t;
    uint16_t temp;
    uint32_t *picoffsettbl; /* 图片文件offset索引表 */
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
  MX_FATFS_Init();
  MX_SPI2_Init();
  MX_TIM4_Init();
  /* USER CODE BEGIN 2 */
  lcd_init();                         		/* 初始化LCD */
  norflash_init();                    		/* 初始化NORFLASH */
  my_mem_init(SRAMIN);               		/* 初始化内部SRAM内存池 */
  exfuns_init();                 			/* 为fatfs相关变量申请内存 */

  f_mount(fs[0], "0:", 1);       			/* 挂载SD卡 */
  f_mount(fs[1], "1:", 1);       			/* 挂载SPI_FLASH卡 */

  while (fonts_init())                		/* 检查字库 */
  {
      lcd_show_string(30, 50, 200, 16, 16, "Font Error!", RED);
      delay_ms(200);
      lcd_fill(30, 50, 240, 66, WHITE); 	/* 清除显示 */
      delay_ms(200);
  }

  text_show_string(30,  50, 200, 16, "正点原子STM32开发板", 16, 0, RED);
  text_show_string(30,  70, 200, 16, "图片显示 实验", 16, 0, RED);
  text_show_string(30,  90, 200, 16, "KEY0:NEXT KEY1:PREV", 16, 0, RED);
  text_show_string(30, 110, 200, 16, "KEY_UP:PAUSE", 16, 0, RED);
  text_show_string(30, 130, 200, 16, "正点原子@ALIENTEK", 16, 0, RED);

  while (f_opendir(&picdir, "0:/PICTURE")) /* 打开图片文件夹 */
  {
	  text_show_string(30, 150, 240, 16, "PICTURE文件夹错误!", 16, 0, RED);
	  delay_ms(200);
	  lcd_fill(30, 150, 240, 186, WHITE); 	/* 清除显示 */
	  delay_ms(200);
  }

  totpicnum = pic_get_tnum("0:/PICTURE"); 	/* 得到总有效文件数 */

  while (totpicnum == NULL) 				/* 图片文件为0 */
  {
	  text_show_string(30, 150, 240, 16, "没有图片文件!", 16, 0, RED);
	  delay_ms(200);
	  lcd_fill(30, 150, 240, 186, WHITE); 	/* 清除显示 */
	  delay_ms(200);
  }
  picfileinfo.lfsize = _MAX_LFN * 2 + 1;						/* 长文件名最大长度 */
  picfileinfo.lfname = mymalloc(SRAMIN, picfileinfo.lfsize);	/* 为长文件缓存区分配内存 */
  pname=mymalloc(SRAMIN, picfileinfo.lfsize);					/* 为带路径的文件名分配内存 */
  picoffsettbl = mymalloc(SRAMIN, 2 * totpicnum);             	/* 申请4*totpicnum个字节的内存,用于存放图片索引 */

  while (!picfileinfo.lfname || !pname || !picoffsettbl) 		/* 内存分配出错 */
  {
	  text_show_string(30, 150, 240, 16, "内存分配失败!", 16, 0, RED);
	  delay_ms(200);
	  lcd_fill(30, 150, 240, 186, WHITE); /* 清除显示 */
	  delay_ms(200);
  }

  /* 记录索引 */
  res = f_opendir(&picdir, "0:/PICTURE"); /* 打开目录 */

  if (res == FR_OK)
  {
	  curindex = 0; /* 当前索引为0 */

	  while (1) /* 全部查询一遍 */
	  {
		  temp = picdir.index;                    /* 记录当前dptr偏移 */
		  res = f_readdir(&picdir, &picfileinfo); /* 读取目录下的一个文件 */

		  if (res != FR_OK || picfileinfo.fname[0] == 0)
			  break; /* 错误了/到末尾了,退出 */
		  fn = (uint8_t*)(*picfileinfo.lfname? picfileinfo.lfname : picfileinfo.fname);
		  res = exfuns_file_type((char *)fn);

		  if ((res & 0XF0) == 0X50) 			/* 取高四位,看看是不是图片文件 */
		  {
			  picoffsettbl[curindex] = temp; 	/* 记录索引 */
			  curindex++;
		  }
	  }
  }

  text_show_string(30, 150, 240, 16, "开始显示...", 16, 0, RED);
  delay_ms(1500);
  piclib_init();                                         /* 初始化画图 */
  curindex = 0;                                          /* 从0开始显示 */
  res = f_opendir(&picdir, (const TCHAR *)"0:/PICTURE"); /* 打开目录 */

  while (res == FR_OK) /* 打开成功 */
  {
	  dir_sdi(&picdir, picoffsettbl[curindex]); 		/* 改变当前目录索引 */
	  res = f_readdir(&picdir, &picfileinfo);    		/* 读取目录下的一个文件 */

	  if (res != FR_OK || picfileinfo.fname[0] == 0)
		  break; /* 错误了/到末尾了,退出 */
	  fn = (uint8_t*)(*picfileinfo.lfname? picfileinfo.lfname : picfileinfo.fname);
	  strcpy((char *)pname, "0:/PICTURE/");          	/* 复制路径(目录) */
	  strcat((char *)pname, (const char *)fn); 			/* 将文件名接在后面 */
	  lcd_clear(BLACK);
	  piclib_ai_load_picfile(pname, 0, 0, lcddev.width, lcddev.height, 1); /* 显示图片 */
	  text_show_string(2, 2, lcddev.width, 16, (char *)pname, 16, 1, RED); /* 显示图片名字 */
	  t = 0;
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
      key = key_scan(0); /* 扫描按键 */

      if (t > 250)
          key = 1; /* 模拟一次按下KEY0 */

      if ((t % 20) == 0)
      {
          LED0_TOGGLE(); /* LED0闪烁,提示程序正在运行. */
      }

      if (key == KEY1_PRES) /* 上一张 */
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
      else if (key == KEY0_PRES) /* 下一张 */
      {
          curindex++;

          if (curindex >= totpicnum)
              curindex = 0; /* 到末尾的时候,自动从头开始 */

          break;
      }
      else if (key == WKUP_PRES)
      {
          pause = !pause;
          LED1(!pause); /* 暂停的时候LED1亮. */
      }

      if (pause == 0)
          t++;

      delay_ms(10);
    /* USER CODE END WHILE */

    /* USER CODE BEGIN 3 */
  }
  res = 0;
  }
  myfree(SRAMIN, picfileinfo.lfname);  	/* 释放内存 */
  myfree(SRAMIN, pname);        		/* 释放内存 */
  myfree(SRAMIN, picoffsettbl); 		/* 释放内存 */
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
