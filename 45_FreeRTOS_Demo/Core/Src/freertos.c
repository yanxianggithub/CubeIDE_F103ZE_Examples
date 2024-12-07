/* USER CODE BEGIN Header */
/* USER CODE END Header */

/* Includes ------------------------------------------------------------------*/
#include "FreeRTOS.h"
#include "task.h"
#include "main.h"
#include "cmsis_os.h"

/* Private includes ----------------------------------------------------------*/
/* USER CODE BEGIN Includes */
#include "../../BSP/LED/led.h"
#include "usart.h"
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
/* USER CODE BEGIN Variables */
/* USER CODE END Variables */
/* Definitions for task01 */
osThreadId_t task01Handle;
const osThreadAttr_t task01_attributes = {
  .name = "task01",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityNormal,
};
/* Definitions for task02 */
osThreadId_t task02Handle;
const osThreadAttr_t task02_attributes = {
  .name = "task02",
  .stack_size = 256 * 4,
  .priority = (osPriority_t) osPriorityLow2,
};

/* Private function prototypes -----------------------------------------------*/
/* USER CODE BEGIN FunctionPrototypes */
/* USER CODE END FunctionPrototypes */

void mytask01(void *argument);
void mytask02(void *argument);

void MX_FREERTOS_Init(void); /* (MISRA C 2004 rule 8.1) */

/**
  * @brief  FreeRTOS initialization
  * @param  None
  * @retval None
  */
void MX_FREERTOS_Init(void) {
  /* USER CODE BEGIN Init */
  /* USER CODE END Init */

  /* USER CODE BEGIN RTOS_MUTEX */
  /* USER CODE END RTOS_MUTEX */

  /* USER CODE BEGIN RTOS_SEMAPHORES */
  /* USER CODE END RTOS_SEMAPHORES */

  /* USER CODE BEGIN RTOS_TIMERS */
  /* USER CODE END RTOS_TIMERS */

  /* USER CODE BEGIN RTOS_QUEUES */
  /* USER CODE END RTOS_QUEUES */

  /* Create the thread(s) */
  /* creation of task01 */
  task01Handle = osThreadNew(mytask01, NULL, &task01_attributes);

  /* creation of task02 */
  task02Handle = osThreadNew(mytask02, NULL, &task02_attributes);

  /* USER CODE BEGIN RTOS_THREADS */
  /* USER CODE END RTOS_THREADS */

  /* USER CODE BEGIN RTOS_EVENTS */
  /* USER CODE END RTOS_EVENTS */

}

/* USER CODE BEGIN Header_mytask01 */
/* USER CODE END Header_mytask01 */
void mytask01(void *argument)
{
  /* USER CODE BEGIN mytask01 */
	while(1)
	{
		LED0_TOGGLE();
		vTaskDelay(1000);
	}

  /* USER CODE END mytask01 */
}

/* USER CODE BEGIN Header_mytask02 */
/* USER CODE END Header_mytask02 */
void mytask02(void *argument)
{
  /* USER CODE BEGIN mytask02 */
	float num_float = 0.0;
	while(1)
	{
		num_float += 0.01;
		printf("num_float:%.2f\r\n",num_float);
		vTaskDelay(500);
	}
  /* USER CODE END mytask02 */
}

/* Private application code --------------------------------------------------*/
/* USER CODE BEGIN Application */
/* USER CODE END Application */

