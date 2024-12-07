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
#include "../../BSP/BEEP/beep.h"
#include "../../BSP/KEY/key.h"
#include "../../BSP/LCD/lcd.h"
#include "../../BSP/NORFLASH/norflash.h"
#include "../../SYSTEM/delay/delay.h"
#include "../../BSP/OV7725/ov7725.h"
#include "../../ATK_Middlewares/MALLOC/malloc.h"
#include "../../ATK_Middlewares/PICTURE/piclib.h"
#include "../../FatFs/exfuns/exfuns.h"
#include "../../ATK_Middlewares/TEXT/text.h"
#include "ff.h"
#include "math.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

extern uint8_t g_ov7725_vsta;       /* 在exit.c里 面定义 */
extern uint8_t g_ov7725_frame;      /* 在timer.c里面定义 */

/*
 * 受限于OV7725模组的FIFO的容量，无法存下一帧640*480的画像，我们这里QVGA和VGA模式都采用
 * 320*240的分辨率,VGA模式下支持缩放，所以可以使用更小的分辨率来显示，读者可以根据需要自行修改
 */
uint16_t g_ov7725_wwidth  = 320;    /* 默认窗口宽度为320 */
uint16_t g_ov7725_wheight = 240;    /* 默认窗口高度为240 */

const char *LMODE_TBL[6]   = {"Auto", "Sunny", "Cloudy", "Office", "Home", "Night"};                    /* 6种光照模式 */
const char *EFFECTS_TBL[7] = {"Normal", "Negative", "B&W", "Redish", "Greenish", "Bluish", "Antique"};  /* 7种特效 */

/**
 * @brief       更新LCD显示
 *   @note      该函数将OV7725模块FIFO里面的数据拷贝到LCD屏幕上
 * @param       无
 * @retval      无
 */
void ov7725_camera_refresh(void)
{
    uint32_t i, j;
    uint16_t color;

    if (g_ov7725_vsta)                  /* 有帧中断更新 */
    {
        lcd_scan_dir(U2D_L2R);          /* 从上到下, 从左到右 */
        lcd_set_window((lcddev.width - g_ov7725_wwidth) / 2, (lcddev.height - g_ov7725_wheight) / 2,
                        g_ov7725_wwidth, g_ov7725_wheight);     /* 将显示区域设置到屏幕中央 */

        lcd_write_ram_prepare();        /* 开始写入GRAM */

        OV7725_RRST(0);                 /* 开始复位读指针 */
        OV7725_RCLK(0);
        OV7725_RCLK(1);
        OV7725_RCLK(0);
        OV7725_RRST(1);                 /* 复位读指针结束 */
        OV7725_RCLK(1);

        for (i = 0; i < g_ov7725_wheight; i++)
        {
            for (j = 0; j < g_ov7725_wwidth; j++)
            {
                OV7725_RCLK(0);
                color = OV7725_DATA;    /* 读数据 */
                OV7725_RCLK(1);
                color <<= 8;
                OV7725_RCLK(0);
                color |= OV7725_DATA;   /* 读数据 */
                OV7725_RCLK(1);
                LCD->LCD_RAM = color;
            }
        }

        g_ov7725_vsta = 0;              /* 清零帧中断标记 */
        g_ov7725_frame++;
        lcd_scan_dir(DFT_SCAN_DIR);     /* 恢复默认扫描方向 */
    }
}

/**
 * @brief       文件名自增（避免覆盖）
 *   @note      组合成形如 "0:PHOTO/PIC13141.bmp" 的文件名
 * @param       pname : 有效的文件名
 * @retval      无
 */
void camera_new_pathname(char *pname)
{
    uint8_t res;
    uint16_t index = 0;
    FIL *ftemp;

    ftemp = (FIL *)mymalloc(SRAMIN, sizeof(FIL));   /* 开辟FIL字节的内存区域 */

    if (ftemp == NULL) return;                      /* 内存申请失败 */

    while (index < 0XFFFF)
    {
        sprintf((char *)pname, "0:PHOTO/PIC%05d.bmp", index);
        res = f_open(ftemp, (const TCHAR *)pname, FA_READ); /* 尝试打开这个文件 */

        if (res == FR_NO_FILE)break;    /* 该文件名不存在, 正是我们需要的 */

        index++;
    }
    myfree(SRAMIN, ftemp);
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
    char *pname;                                /* 带路径的文件名 */
    uint8_t key;                                /* 键值 */
    uint8_t i;
    uint8_t sd_ok = 1;                          /* 0, sd卡不正常; 1, SD卡正常 */
    uint8_t vga_mode = 0;                       /* 0, QVGA模式(320 * 240); 1, VGA模式(640 * 480) */

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
  MX_TIM6_Init();
  /* USER CODE BEGIN 2 */
  lcd_init();                         		/* 初始化LCD */
  norflash_init();                    		/* 初始化NORFLASH */
  my_mem_init(SRAMIN);               		/* 初始化内部SRAM内存池 */
  exfuns_init();                 			/* 为fatfs相关变量申请内存 */

  f_mount(fs[0], "0:", 1);       			/* 挂载SD卡 */
  f_mount(fs[1], "1:", 1);       			/* 挂载SPI_FLASH卡 */

  piclib_init();                         	/* 初始化画图 */
  while (fonts_init())                    	/* 检查字库 */
  {
      lcd_show_string(30, 50, 200, 16, 16, "Font Error!", RED);
      delay_ms(200);
      lcd_fill(30, 50, 240, 66, WHITE);   	/* 清除显示 */
      delay_ms(200);
  }

  text_show_string(30, 50, 200, 16, "正点原子STM32开发板", 16, 0, RED);
  text_show_string(30, 70, 200, 16, "照相机 实验", 16, 0, RED);
  text_show_string(30, 90, 200, 16, "KEY0:拍照(bmp格式)", 16, 0, RED);
  res = f_mkdir("0:/PHOTO");             	/* 创建PHOTO文件夹 */

  if (res != FR_EXIST && res != FR_OK)    	/* 发生了错误 */
  {
      res = f_mkdir("0:/PHOTO");          	/* 创建PHOTO文件夹 */
      text_show_string(30, 110, 240, 16, "SD卡错误!", 16, 0, RED);
      delay_ms(200);
      text_show_string(30, 110, 240, 16, "拍照功能将不可用!", 16, 0, RED);
      delay_ms(200);
      sd_ok = 0;
  }

  while (ov7725_init() != 0)  				/* 初始化OV7725 失败? */
  {
      lcd_show_string(30, 130, 200, 16, 16, "OV7725 Error!!", RED);
      delay_ms(200);
      lcd_fill(30, 130, 239, 246, WHITE);
      delay_ms(200);
  }

  lcd_show_string(30, 130, 200, 16, 16, "OV7725 Init OK       ", RED);

  delay_ms(1500);

  /* 输出窗口大小设置 QVGA / VGA 模式 */
  g_ov7725_wwidth = 320;                  /* 默认窗口宽度为320 */
  g_ov7725_wheight = 240;                 /* 默认窗口高度为240 */
  ov7725_window_set(g_ov7725_wwidth, g_ov7725_wheight, vga_mode);

  ov7725_light_mode(0);                   /* 自动 灯光模式 */
  ov7725_color_saturation(4);             /* 默认 色彩饱和度 */
  ov7725_brightness(4);                   /* 默认 亮度 */
  ov7725_contrast(4);                     /* 默认 对比度 */
  ov7725_special_effects(0);              /* 默认 特效 */

  OV7725_OE(0);                           /* 使能OV7725 FIFO数据输出 */

  pname = mymalloc(SRAMIN, 30);           /* 为带路径的文件名分配30个字节的内存 */
  lcd_clear(BLACK);

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  key = key_scan(0);
	  if (key == KEY0_PRES)
	  {
		  if (sd_ok)
		  {
			  LED1(0);                    /* 点亮LED1,提示正在拍照 */
			  camera_new_pathname(pname); /* 得到文件名 */

			  /* 编码成bmp图片 */
			  if (bmp_encode((uint8_t *)pname, (lcddev.width - g_ov7725_wheight) / 2, (lcddev.height - g_ov7725_wwidth) / 2, g_ov7725_wheight, g_ov7725_wwidth, 0))
			  {
				  text_show_string(40, 110, 240, 12, "写入文件错误!", 12, 0, RED);
			  }
			  else
			  {
				  text_show_string(40, 110, 240, 12, "拍照成功!", 12, 0, BLUE);
				  text_show_string(40, 130, 240, 12, "保存为:", 12, 0, BLUE);
				  text_show_string(40 + 42, 130, 240, 12, pname, 12, 0, BLUE);
				  BEEP(1);        /* 蜂鸣器短叫，提示拍照完成 */
				  delay_ms(100);
			  }
		  }
		  else     /* 提示SD卡错误 */
		  {
			  text_show_string(40, 110, 240, 12, "SD卡错误!", 12, 0, RED);
			  text_show_string(40, 130, 240, 12, "拍照功能不可用!", 12, 0, RED);
		  }

		  BEEP(0);        /* 关闭蜂鸣器 */
		  LED1(1);        /* 关闭LED1 */
		  delay_ms(1800); /* 等待1.8秒钟 */
		  lcd_clear(BLACK);
	  }
	  else
	  {
		  delay_ms(5);
	  }

	  ov7725_camera_refresh();/* 更新显示 */

	  i++;

	  if (i >= 15)
	  {
		  i = 0;
		  LED0_TOGGLE();      /* LED0闪烁 */
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
