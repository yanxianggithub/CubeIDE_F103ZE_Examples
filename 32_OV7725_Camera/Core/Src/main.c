/* USER CODE BEGIN Header */
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
#include "../../BSP/KEY/key.h"
#include "../../BSP/LCD/lcd.h"
#include "../../BSP/TPAD/tpad.h"
#include "../../BSP/OV7725/ov7725.h"
#include "../../SYSTEM/delay/delay.h"
/* USER CODE END Includes */

/* Private typedef -----------------------------------------------------------*/
/* USER CODE BEGIN PTD */
extern uint8_t g_ov7725_vsta;  /* 在exit.c里 面定义 */
extern uint8_t g_ov7725_frame; /* 在timer.c里面定义 */
/* USER CODE END PTD */

/* Private define ------------------------------------------------------------*/
/* USER CODE BEGIN PD */
/*
 * 受限于OV7725模组的FIFO的容量，无法存下一帧640*480的画像，我们这里QVGA和VGA模式都采用
 * 320*240的分辨率,VGA模式下支持缩放，所以可以使用更小的分辨率来显示，读者可以根据需要自行修改
 */
uint16_t g_ov7725_wwidth = 320;  /* 默认窗口宽度为320 */
uint16_t g_ov7725_wheight = 240; /* 默认窗口高度为240 */

const char *LMODE_TBL[6] = {"Auto", "Sunny", "Cloudy", "Office", "Home", "Night"};                     /* 6种光照模式 */
const char *EFFECTS_TBL[7] = {"Normal", "Negative", "B&W", "Redish", "Greenish", "Bluish", "Antique"}; /* 7种特效 */

/* USER CODE END PD */

/* Private macro -------------------------------------------------------------*/
/* USER CODE BEGIN PM */

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

    if (g_ov7725_vsta) /* 有帧中断更新 */
    {
        lcd_scan_dir(U2D_L2R); /* 从上到下, 从左到右 */
        lcd_set_window((lcddev.width - g_ov7725_wwidth) / 2, (lcddev.height - g_ov7725_wheight) / 2,
                       g_ov7725_wwidth, g_ov7725_wheight); /* 将显示区域设置到屏幕中央 */


        lcd_write_ram_prepare(); /* 开始写入GRAM */

        OV7725_RRST(0); /* 开始复位读指针 */
        OV7725_RCLK(0);
        OV7725_RCLK(1);
        OV7725_RCLK(0);
        OV7725_RRST(1); /* 复位读指针结束 */
        OV7725_RCLK(1);

        for (i = 0; i < g_ov7725_wheight; i++)
        {
            for (j = 0; j < g_ov7725_wwidth; j++)
            {
                OV7725_RCLK(0);
                color = OV7725_DATA; /* 读数据 */
                OV7725_RCLK(1);
                color <<= 8;
                OV7725_RCLK(0);
                color |= OV7725_DATA; /* 读数据 */
                OV7725_RCLK(1);
                LCD->LCD_RAM = color;
            }
        }

        g_ov7725_vsta = 0; /* 清零帧中断标记 */
        g_ov7725_frame++;
        lcd_scan_dir(DFT_SCAN_DIR); /* 恢复默认扫描方向 */
    }
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
    uint8_t i = 0;
    char msgbuf[15]; /* 消息缓存区 */
    uint8_t tm = 0;
    uint8_t lightmode = 0, effect = 0;
    uint8_t saturation = 4, brightness = 4, contrast = 4;
  /* USER CODE END 1 */

  /* MCU Configuration--------------------------------------------------------*/

  /* Reset of all peripherals, Initializes the Flash interface and the Systick. */
  HAL_Init();

  /* USER CODE BEGIN Init */

  /* USER CODE END Init */

  /* Configure the system clock */
  SystemClock_Config();

  /* USER CODE BEGIN SysInit */
  delay_init(72);						/* 初始化延时函数 */
  /* USER CODE END SysInit */

  /* Initialize all configured peripherals */
  MX_GPIO_Init();
  MX_FSMC_Init();
  MX_USART1_UART_Init();
  MX_TIM5_Init();
  MX_TIM6_Init();
  /* USER CODE BEGIN 2 */
  lcd_init();                     		/* 初始化LCD */
  tpad_init();                       	/* TPAD 初始化 */
  lcd_show_string(30,  50, 200, 16, 16, "STM32", RED);
  lcd_show_string(30,  70, 200, 16, 16, "OV7725 TEST", RED);
  lcd_show_string(30,  90, 200, 16, 16, "ATOM@ALIENTEK", RED);
  lcd_show_string(30, 110, 200, 16, 16, "KEY0:Light Mode", RED);
  lcd_show_string(30, 130, 200, 16, 16, "KEY1:Saturation", RED);
  lcd_show_string(30, 150, 200, 16, 16, "KEY_UP:Contrast", RED);
  lcd_show_string(30, 170, 200, 16, 16, "TPAD:Effects", RED);
  lcd_show_string(30, 190, 200, 16, 16, "OV7725 Init...", RED);
  /* USER CODE END 2 */

  /* Infinite loop */
  /* USER CODE BEGIN WHILE */
  while (1)
  {
	  if (ov7725_init() == 0)
	  {
		  lcd_show_string(30, 190, 200, 16, 16, "OV7725 Init OK       ", RED);

		  while (1)
		  {
			  key = key_scan(0);

			  if (key == KEY0_PRES)
			  {
				  g_ov7725_wwidth = 320;                                  /* 默认窗口宽度为320 */
				  g_ov7725_wheight = 240;                                 /* 默认窗口高度为240 */
				  ov7725_window_set(g_ov7725_wwidth, g_ov7725_wheight, 0);/* QVGA模式输出 */
				  break;
			  }
			  else if (key == KEY1_PRES)
			  {
				  g_ov7725_wwidth = 320;                                  /* 默认窗口宽度为320 */
				  g_ov7725_wheight = 240;                                 /* 默认窗口高度为240 */
				  ov7725_window_set(g_ov7725_wwidth, g_ov7725_wheight, 1);/* VGA模式输出 */
				  break;
			  }

			  i++;

			  if (i == 100)
				  lcd_show_string(30, 210, 210, 16, 16, "KEY0:QVGA  KEY1:VGA", RED); /* 闪烁显示提示信息 */

			  if (i == 200)
			  {
				  lcd_fill(30, 230, 210, 250 + 16, WHITE);
				  i = 0;
			  }

			  delay_ms(5);
		  }

		  ov7725_light_mode(lightmode);
		  ov7725_color_saturation(saturation);
		  ov7725_brightness(brightness);
		  ov7725_contrast(contrast);
		  ov7725_special_effects(effect);

		  OV7725_OE(0); /* 使能OV7725 FIFO数据输出 */

		  break;
	  }
	  else
	  {
		  lcd_show_string(30, 190, 200, 16, 16, "OV7725 Error!!", RED);
		  delay_ms(200);
		  lcd_fill(30, 190, 239, 246, WHITE);
		  delay_ms(200);
	  }
  }
  lcd_clear(BLACK);

  while (1)
  {
	  key = key_scan(0); /* 不支持连按 */

	  if (key)
	  {
		  tm = 20;

		  switch (key)
		  {
			  case KEY0_PRES: /* 灯光模式Light Mode */
				  lightmode++;

				  if (lightmode > 5)
					  lightmode = 0;

				  ov7725_light_mode(lightmode);
				  sprintf((char *)msgbuf, "%s", LMODE_TBL[lightmode]);
				  break;

			  case KEY1_PRES: /* 饱和度Saturation */
				  saturation++;

				  if (saturation > 8)
					  saturation = 0;

				  ov7725_color_saturation(saturation);
				  sprintf((char *)msgbuf, "Saturation:%d", saturation);
				  break;

			  case WKUP_PRES: /* 对比度Contrast */
				  contrast++;

				  if (contrast > 8)
					  contrast = 0;

				  ov7725_contrast(contrast);
				  sprintf((char *)msgbuf, "Contrast:%d", contrast);
				  break;
			  }
	  }

	  if (tpad_scan(0)) /* 检测到触摸按键 */
	  {
		  effect++;

		  if (effect > 6)
			  effect = 0;

		  ov7725_special_effects(effect); /* 设置特效 */
		  sprintf((char *)msgbuf, "%s", EFFECTS_TBL[effect]);
		  tm = 20;
	  }

	  ov7725_camera_refresh(); /* 更新显示 */

	  if (tm)
	  {
		  lcd_show_string((lcddev.width - 240) / 2 + 30, (lcddev.height - 320) / 2 + 60, 200, 16, 16, msgbuf, BLUE);
		  tm--;
	  }

	  i++;

	  if (i >= 15) /* DS0闪烁 */
	  {
		  i = 0;
		  LED0_TOGGLE(); /* LED0闪烁 */
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
