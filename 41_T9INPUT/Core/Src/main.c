/* USER CODE BEGIN Header */
/**
 ****************************************************************************************************
 * @file        main.c
 * @author      正点原子团队(ALIENTEK)
 * @version     V1.0
 * @date        2024-01-01
 * @brief       T9拼音输入法 实验
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
#include "../../BSP/TOUCH/touch.h"
#include "../../SYSTEM/delay/delay.h"
#include "../../ATK_Middlewares/MALLOC/malloc.h"
#include "../../FatFs/exfuns/exfuns.h"
#include "../../ATK_Middlewares/TEXT/text.h"
#include "../../ATK_Middlewares/T9INPUT/pyinput.h"
#include "ff.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */

/* 数字表 */
const char *kbd_tbl[9] = {"←", "2", "3", "4", "5", "6", "7", "8", "9"};

/* 字符表 */
const char *kbs_tbl[9] = {"DEL", "abc", "def", "ghi", "jkl", "mno", "pqrs", "tuv", "wxyz"};

uint16_t kbdxsize;  /* 虚拟键盘按键宽度 */
uint16_t kbdysize;  /* 虚拟键盘按键高度 */

/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */


/**
 * @brief       加载键盘界面
 * @param       x, y : 界面起始坐标
 * @retval      无
 */
void py_load_ui(uint16_t x, uint16_t y)
{
    uint16_t i;
    lcd_draw_rectangle(x, y, x + kbdxsize * 3, y + kbdysize * 3, RED);
    lcd_draw_rectangle(x + kbdxsize, y, x + kbdxsize * 2, y + kbdysize * 3, RED);
    lcd_draw_rectangle(x, y + kbdysize, x + kbdxsize * 3, y + kbdysize * 2, RED);

    for (i = 0; i < 9; i++)
    {
        text_show_string_middle(x + (i % 3)*kbdxsize, y + 4 + kbdysize * (i / 3), (char *)kbd_tbl[i], 16, kbdxsize, BLUE);
        text_show_string_middle(x + (i % 3)*kbdxsize, y + kbdysize / 2 + kbdysize * (i / 3), (char *)kbs_tbl[i], 16, kbdxsize, BLUE);
    }
}

/**
 * @brief       按键状态设置
 * @param       x, y : 键盘坐标
 * @param       key  : 键值（0~8）
 * @retval      对应按键状态
 *   @arg       0, 松开;
 *   @arg       1, 按下;
 */
void py_key_staset(uint16_t x, uint16_t y, uint8_t keyx, uint8_t sta)
{
    uint16_t i = keyx / 3, j = keyx % 3;

    if (keyx > 8)return;

    if (sta)lcd_fill(x + j * kbdxsize + 1, y + i * kbdysize + 1, x + j * kbdxsize + kbdxsize - 1, y + i * kbdysize + kbdysize - 1, GREEN);
    else lcd_fill(x + j * kbdxsize + 1, y + i * kbdysize + 1, x + j * kbdxsize + kbdxsize - 1, y + i * kbdysize + kbdysize - 1, WHITE);

    text_show_string_middle(x + j * kbdxsize, y + 4 + kbdysize * i, (char *)kbd_tbl[keyx], 16, kbdxsize, BLUE);
    text_show_string_middle(x + j * kbdxsize, y + kbdysize / 2 + kbdysize * i, (char *)kbs_tbl[keyx], 16, kbdxsize, BLUE);
}

/**
 * @brief       得到触摸屏的输入
 * @param       x, y : 键盘坐标
 * @retval      按键键值(1~9 有效；0, 无效;)
 */
uint8_t py_get_keynum(uint16_t x, uint16_t y)
{
    uint16_t i, j;
    static uint8_t key_x = 0;       /* 0,没有任何按键按下；1~9，1~9号按键按下 */
    uint8_t key = 0;
    tp_dev.scan(0);

    if (tp_dev.sta & TP_PRES_DOWN)  /* 触摸屏被按下 */
    {
        for (i = 0; i < 3; i++)
        {
            for (j = 0; j < 3; j++)
            {
                if (tp_dev.x[0] < (x + j * kbdxsize + kbdxsize) &&
                    tp_dev.x[0] > (x + j * kbdxsize) &&
                    tp_dev.y[0] < (y + i * kbdysize + kbdysize) &&
                    tp_dev.y[0] > (y + i * kbdysize))
                {
                    key = i * 3 + j + 1;
                    break;
                }
            }

            if (key)
            {
                if (key_x == key)key = 0;
                else
                {
                    py_key_staset(x, y, key_x - 1, 0);
                    key_x = key;
                    py_key_staset(x, y, key_x - 1, 1);
                }

                break;
            }
        }
    }
    else if (key_x)
    {
        py_key_staset(x, y, key_x - 1, 0);
        key_x = 0;
    }

    return key;
}

/**
 * @brief       显示结果
 * @param       index: 索引号
 *   @arg       0    , 表示没有一个匹配的结果, 清空之前的显示
 *   @arg       其他 , 索引号
 * @retval      无
 */
void py_show_result(uint8_t index)
{
    lcd_show_num(30 + 144, 125, index, 1, 16, BLUE);            /* 显示当前的索引 */
    lcd_fill(30 + 40, 125, 30 + 40 + 48, 130 + 16, WHITE);      /* 清除之前的显示 */
    lcd_fill(30 + 40, 145, lcddev.width - 1, 145 + 48, WHITE);  /* 清除之前的显示 */

    if (index)
    {
        text_show_string(30 + 40, 125, 200, 16, (char *)t9.pymb[index - 1]->py, 16, 0, BLUE);   /* 显示拼音 */
        text_show_string(30 + 40, 145, lcddev.width - 70, 48, (char *)t9.pymb[index - 1]->pymb, 16, 0, BLUE);   /* 显示对应的汉字 */
        printf("\r\n拼音:%s\r\n", t9.pymb[index - 1]->py);    /* 串口输出拼音 */
        printf("结果:%s\r\n", t9.pymb[index - 1]->pymb);      /* 串口输出结果 */
    }
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
    uint8_t i = 0;
    uint8_t result_num;
    uint8_t cur_index;
    uint8_t key;
    uint8_t temp;
    uint8_t inputstr[7];                        /* 最大输入6个字符+结束符 */
    uint8_t inputlen;                           /* 输入长度 */
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
  tp_dev.init();                            /* 初始化触摸屏 */
  my_mem_init(SRAMIN);               		/* 初始化内部SRAM内存池 */
  exfuns_init();                 			/* 为fatfs相关变量申请内存 */

  f_mount(fs[0], "0:", 1);       			/* 挂载SD卡 */
  f_mount(fs[1], "1:", 1);       			/* 挂载SPI_FLASH卡 */

RESTART:
  while (fonts_init())                        /* 检查字库 */
  {
	  lcd_show_string(60, 50, 200, 16, 16, "Font Error!", RED);
	  delay_ms(200);
	  lcd_fill(60, 50, 240, 66, WHITE);   /* 清除显示 */
	  delay_ms(200);
  }
  text_show_string(30, 5, 200, 16, "正点原子STM32开发板", 16, 0, RED);
  text_show_string(30, 25, 200, 16, "拼音输入法实验", 16, 0, RED);
  text_show_string(30, 45, 200, 16, "正点原子@ALIENTEK", 16, 0, RED);
  text_show_string(30, 65, 200, 16, "KEY_UP:校准", 16, 0, RED);
  text_show_string(30, 85, 200, 16, "KEY0:翻页  KEY1:清除", 16, 0, RED);
  text_show_string(30, 105, 200, 16, "输入:        匹配:  ", 16, 0, RED);
  text_show_string(30, 125, 200, 16, "拼音:        当前:  ", 16, 0, RED);
  text_show_string(30, 145, 210, 32, "结果:", 16, 0, RED);

  /* 根据LCD分辨率设置按键大小 */
  if (lcddev.id == 0X5310)
  {
	  kbdxsize = 86;
	  kbdysize = 43;
  }
  else if (lcddev.id == 0X5510)
  {
	  kbdxsize = 140;
	  kbdysize = 70;
  }
  else
  {
	  kbdxsize = 60;
	  kbdysize = 40;
  }

  py_load_ui(30, 195);
  my_mem_set(inputstr, 0, 7); /* 全部清零 */
  inputlen = 0;   /* 输入长度为0 */
  result_num = 0; /* 总匹配数清零 */
  cur_index = 0;

  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  i++;
	  delay_ms(10);
	  key = py_get_keynum(30, 195);

	  if (key)
	  {
		  if (key == 1)   /* 删除 */
		  {
			  if (inputlen)inputlen--;

			  inputstr[inputlen] = '\0';      /* 添加结束符 */
		  }
		  else
		  {
			  inputstr[inputlen] = key + '0'; /* 输入字符 */

			  if (inputlen < 7)inputlen++;
		  }

		  if (inputstr[0] != NULL)
		  {
			  temp = t9.getpymb(inputstr);     /* 得到匹配的结果数 */

			  if (temp)   /* 有部分匹配/完全匹配的结果 */
			  {
				  result_num = temp & 0X7F;   /* 总匹配结果 */
				  cur_index = 1;              /* 当前为第一个索引 */

				  if (temp & 0X80)            /* 是部分匹配 */
				  {
					  inputlen = temp & 0X7F; /* 有效匹配位数 */
					  inputstr[inputlen] = '\0';  /* 不匹配的位数去掉 */

					  if (inputlen > 1)
					  {
						  temp = t9.getpymb(inputstr);    /* 重新获取完全匹配字符数 */
						  result_num = (temp & 0X80)? 0 : (temp & 0X7F); /* 如果还是部分匹配, 直接匹配数为0, 否则表示匹配数量 */
					  }
				  }
			  }
			  else    /* 没有任何匹配 */
			  {
				  inputlen--;
				  inputstr[inputlen] = '\0';
			  }
		  }
		  else
		  {
			  cur_index = 0;
			  result_num = 0;
		  }

		  lcd_fill(30 + 40, 105, 30 + 40 + 48, 110 + 16, WHITE);      /* 清除之前的显示 */
		  lcd_show_num(30 + 144, 105, result_num, 1, 16, BLUE);       /* 显示匹配的结果数 */
		  text_show_string(30 + 40, 105, 200, 16, (char *)inputstr, 16, 0, BLUE); /* 显示有效的数字串 */
		  py_show_result(cur_index);  /* 显示第cur_index的匹配结果 */
	  }

	  key = key_scan(0);

	  if (key == WKUP_PRES && tp_dev.touchtype == 0)  /* KEYUP按下,且是电阻屏 */
	  {
		  tp_dev.adjust();
		  lcd_clear(WHITE);
		  goto RESTART;
	  }

	  switch (key)
	  {
		  case KEY0_PRES: /* 下翻 */
			  if (result_num) /* 存在匹配的结果 */
			  {
				  if (cur_index < result_num)cur_index++;
				  else cur_index = 1;

				  py_show_result(cur_index);  /* 显示第cur_index的匹配结果 */
			  }
			  break;

		  case KEY1_PRES: /* 清除输入 */
			  lcd_fill(30 + 40, 145, lcddev.width - 1, 145 + 48, WHITE);  /* 清除之前的显示 */
			  goto RESTART;
	  }


	  if (i == 30)
	  {
		  i = 0;
		  LED0_TOGGLE();
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
