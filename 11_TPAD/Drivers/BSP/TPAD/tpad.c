
#include "tpad.h"
#include "../../SYSTEM/delay/delay.h"
#include "usart.h"
#include "tim.h"

/* 空载的时候(没有手按下),计数器需要的时间
 * 这个值应该在每次开机的时候被初始化一次
 */
volatile uint16_t g_tpad_default_val = 0;   /* 空载的时候(没有手按下),计数器需要的时间 */

/* 静态函数, 仅限 tapd.c调用 */
static void tpad_reset(void);               /* 复位 */
static uint16_t tpad_get_val(void);         /* 得到定时器捕获值 */
static uint16_t tpad_get_maxval(uint8_t n); /* 读取n次, 获取最大值 */

/**
 * @brief       初始化触摸按键
 * @param       psc     : 分频系数(值越小, 越灵敏, 最小值为: 1)
 * @retval      0, 初始化成功; 1, 初始化失败;
 */
uint8_t tpad_init(uint16_t psc)
{
    uint16_t buf[10];
    uint16_t temp;
    uint8_t j, i;
    MX_TIM5_Init();

    for (i = 0; i < 10; i++)    /* 连续读取10次 */
    {
        buf[i] = tpad_get_val();
        delay_ms(10);
    }

    for (i = 0; i < 9; i++)     /* 排序 */
    {
        for (j = i + 1; j < 10; j++)
        {
            if (buf[i] > buf[j])/* 升序排列 */
            {
                temp = buf[i];
                buf[i] = buf[j];
                buf[j] = temp;
            }
        }
    }

    temp = 0;

    for (i = 2; i < 8; i++)     /* 取中间的6个数据进行平均 */
    {
        temp += buf[i];
    }

    g_tpad_default_val = temp / 6;
    printf("g_tpad_default_val:%d\r\n", g_tpad_default_val);

    if (g_tpad_default_val > TPAD_ARR_MAX_VAL / 2)
    {
        return 1;   /* 初始化遇到超过TPAD_ARR_MAX_VAL/2的数值,不正常! */
    }

    return 0;
}

/**
 * @brief       复位TPAD
 *   @note      我们将TPAD按键看做是一个电容, 当手指按下/不按下时容值有变化
 *              该函数将GPIO设置成推挽输出, 然后输出0, 进行放电, 然后再设置
 *              GPIO为浮空输入, 等待外部大电阻慢慢充电
 * @param       无
 * @retval      无
 */
static void tpad_reset(void)
{
    GPIO_InitTypeDef gpio_init_struct;
    gpio_init_struct.Pin = TPAD_Pin;                               	/* 输入捕获的GPIO口 */
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;                 	/* 复用推挽输出 */
    gpio_init_struct.Pull = GPIO_PULLUP;                        	/* 上拉 */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_MEDIUM;               	/* 中速 */
    HAL_GPIO_Init(TPAD_GPIO_Port, &gpio_init_struct);

    HAL_GPIO_WritePin(TPAD_GPIO_Port, TPAD_Pin, GPIO_PIN_RESET);   	/* TPAD引脚输出0, 放电 */

    delay_ms(5);

    htim5.Instance->SR = 0;                       					/* 清除标记 */
    htim5.Instance->CNT = 0;                       					/* 归零 */

    gpio_init_struct.Pin = TPAD_Pin;             					/* 输入捕获的GPIO口 */
    gpio_init_struct.Mode = GPIO_MODE_INPUT;          				/* 复用推挽输出 */
    gpio_init_struct.Pull = GPIO_NOPULL;              				/* 浮空 */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_MEDIUM;  				/* 中速 */
    HAL_GPIO_Init(TPAD_GPIO_Port, &gpio_init_struct); 				/* TPAD引脚浮空输入 */
}

/**
 * @brief       得到定时器捕获值
 *   @note      如果超时, 则直接返回定时器的计数值
 *              我们定义超时时间为: TPAD_ARR_MAX_VAL - 500
 * @param       无
 * @retval      捕获值/计数值（超时的情况下返回）
 */
 uint16_t tpad_get_val(void)
{
    uint32_t flag = (TPAD_TIMX_CAP_CHY== TIM_CHANNEL_1)?TIM_FLAG_CC1:\
                    (TPAD_TIMX_CAP_CHY== TIM_CHANNEL_2)?TIM_FLAG_CC2:\
                    (TPAD_TIMX_CAP_CHY== TIM_CHANNEL_3)?TIM_FLAG_CC3:TIM_FLAG_CC4;

    tpad_reset();
    while (__HAL_TIM_GET_FLAG(&htim5 ,flag) == RESET) 			/* 等待通道CHY捕获上升沿 */
    {
        if (htim5.Instance->CNT > TPAD_ARR_MAX_VAL - 500)
        {
            return htim5.Instance->CNT;  						/* 超时了,直接返回CNT的值 */
        }
    }

    return __HAL_TIM_GET_COMPARE(&htim5, TPAD_TIMX_CAP_CHY);  	/* 返回捕获/比较值 */
}

/**
 * @brief       读取n次, 取最大值
 * @param       n       ：连续获取的次数
 * @retval      n次读数里面读到的最大读数值
 */
static uint16_t tpad_get_maxval(uint8_t n)
{
    uint16_t temp = 0;
    uint16_t maxval = 0;

    while (n--)
    {
        temp = tpad_get_val();  /* 得到一次值 */

        if (temp > maxval)maxval = temp;
    }

    return maxval;
}

/**
 * @brief       扫描触摸按键
 * @param       mode    ：扫描模式
 *   @arg       0, 不支持连续触发(按下一次必须松开才能按下一次);
 *   @arg       1, 支持连续触发(可以一直按下)
 * @retval      0, 没有按下; 1, 有按下;
 */
uint8_t tpad_scan(uint8_t mode)
{
    static uint8_t keyen = 0;   /* 0, 可以开始检测;  >0, 还不能开始检测; */
    uint8_t res = 0;
    uint8_t sample = 3;         /* 默认采样次数为3次 */
    uint16_t rval;

    if (mode)
    {
        sample = 6;     /* 支持连按的时候，设置采样次数为6次 */
        keyen = 0;      /* 支持连按, 每次调用该函数都可以检测 */
    }

    rval = tpad_get_maxval(sample);

    if (rval > (g_tpad_default_val + TPAD_GATE_VAL))/* 大于tpad_default_val+TPAD_GATE_VAL,有效 */
    {
        if (keyen == 0)
        {
            res = 1;    /* keyen==0, 有效 */
        }
        keyen = 3;      /* 至少要再过3次之后才能按键有效 */
    }

    if (keyen)keyen--;

    return res;
}

