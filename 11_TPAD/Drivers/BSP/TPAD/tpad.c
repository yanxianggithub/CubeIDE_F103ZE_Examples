
#include "tpad.h"
#include "../../SYSTEM/delay/delay.h"
#include "usart.h"
#include "tim.h"

/* ���ص�ʱ��(û���ְ���),��������Ҫ��ʱ��
 * ���ֵӦ����ÿ�ο�����ʱ�򱻳�ʼ��һ��
 */
volatile uint16_t g_tpad_default_val = 0;   /* ���ص�ʱ��(û���ְ���),��������Ҫ��ʱ�� */

/* ��̬����, ���� tapd.c���� */
static void tpad_reset(void);               /* ��λ */
static uint16_t tpad_get_val(void);         /* �õ���ʱ������ֵ */
static uint16_t tpad_get_maxval(uint8_t n); /* ��ȡn��, ��ȡ���ֵ */

/**
 * @brief       ��ʼ����������
 * @param       psc     : ��Ƶϵ��(ֵԽС, Խ����, ��СֵΪ: 1)
 * @retval      0, ��ʼ���ɹ�; 1, ��ʼ��ʧ��;
 */
uint8_t tpad_init(uint16_t psc)
{
    uint16_t buf[10];
    uint16_t temp;
    uint8_t j, i;
    MX_TIM5_Init();

    for (i = 0; i < 10; i++)    /* ������ȡ10�� */
    {
        buf[i] = tpad_get_val();
        delay_ms(10);
    }

    for (i = 0; i < 9; i++)     /* ���� */
    {
        for (j = i + 1; j < 10; j++)
        {
            if (buf[i] > buf[j])/* �������� */
            {
                temp = buf[i];
                buf[i] = buf[j];
                buf[j] = temp;
            }
        }
    }

    temp = 0;

    for (i = 2; i < 8; i++)     /* ȡ�м��6�����ݽ���ƽ�� */
    {
        temp += buf[i];
    }

    g_tpad_default_val = temp / 6;
    printf("g_tpad_default_val:%d\r\n", g_tpad_default_val);

    if (g_tpad_default_val > TPAD_ARR_MAX_VAL / 2)
    {
        return 1;   /* ��ʼ����������TPAD_ARR_MAX_VAL/2����ֵ,������! */
    }

    return 0;
}

/**
 * @brief       ��λTPAD
 *   @note      ���ǽ�TPAD����������һ������, ����ָ����/������ʱ��ֵ�б仯
 *              �ú�����GPIO���ó��������, Ȼ�����0, ���зŵ�, Ȼ��������
 *              GPIOΪ��������, �ȴ��ⲿ������������
 * @param       ��
 * @retval      ��
 */
static void tpad_reset(void)
{
    GPIO_InitTypeDef gpio_init_struct;
    gpio_init_struct.Pin = TPAD_Pin;                               	/* ���벶���GPIO�� */
    gpio_init_struct.Mode = GPIO_MODE_OUTPUT_PP;                 	/* ����������� */
    gpio_init_struct.Pull = GPIO_PULLUP;                        	/* ���� */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_MEDIUM;               	/* ���� */
    HAL_GPIO_Init(TPAD_GPIO_Port, &gpio_init_struct);

    HAL_GPIO_WritePin(TPAD_GPIO_Port, TPAD_Pin, GPIO_PIN_RESET);   	/* TPAD�������0, �ŵ� */

    delay_ms(5);

    htim5.Instance->SR = 0;                       					/* ������ */
    htim5.Instance->CNT = 0;                       					/* ���� */

    gpio_init_struct.Pin = TPAD_Pin;             					/* ���벶���GPIO�� */
    gpio_init_struct.Mode = GPIO_MODE_INPUT;          				/* ����������� */
    gpio_init_struct.Pull = GPIO_NOPULL;              				/* ���� */
    gpio_init_struct.Speed = GPIO_SPEED_FREQ_MEDIUM;  				/* ���� */
    HAL_GPIO_Init(TPAD_GPIO_Port, &gpio_init_struct); 				/* TPAD���Ÿ������� */
}

/**
 * @brief       �õ���ʱ������ֵ
 *   @note      �����ʱ, ��ֱ�ӷ��ض�ʱ���ļ���ֵ
 *              ���Ƕ��峬ʱʱ��Ϊ: TPAD_ARR_MAX_VAL - 500
 * @param       ��
 * @retval      ����ֵ/����ֵ����ʱ������·��أ�
 */
 uint16_t tpad_get_val(void)
{
    uint32_t flag = (TPAD_TIMX_CAP_CHY== TIM_CHANNEL_1)?TIM_FLAG_CC1:\
                    (TPAD_TIMX_CAP_CHY== TIM_CHANNEL_2)?TIM_FLAG_CC2:\
                    (TPAD_TIMX_CAP_CHY== TIM_CHANNEL_3)?TIM_FLAG_CC3:TIM_FLAG_CC4;

    tpad_reset();
    while (__HAL_TIM_GET_FLAG(&htim5 ,flag) == RESET) 			/* �ȴ�ͨ��CHY���������� */
    {
        if (htim5.Instance->CNT > TPAD_ARR_MAX_VAL - 500)
        {
            return htim5.Instance->CNT;  						/* ��ʱ��,ֱ�ӷ���CNT��ֵ */
        }
    }

    return __HAL_TIM_GET_COMPARE(&htim5, TPAD_TIMX_CAP_CHY);  	/* ���ز���/�Ƚ�ֵ */
}

/**
 * @brief       ��ȡn��, ȡ���ֵ
 * @param       n       ��������ȡ�Ĵ���
 * @retval      n�ζ������������������ֵ
 */
static uint16_t tpad_get_maxval(uint8_t n)
{
    uint16_t temp = 0;
    uint16_t maxval = 0;

    while (n--)
    {
        temp = tpad_get_val();  /* �õ�һ��ֵ */

        if (temp > maxval)maxval = temp;
    }

    return maxval;
}

/**
 * @brief       ɨ�败������
 * @param       mode    ��ɨ��ģʽ
 *   @arg       0, ��֧����������(����һ�α����ɿ����ܰ���һ��);
 *   @arg       1, ֧����������(����һֱ����)
 * @retval      0, û�а���; 1, �а���;
 */
uint8_t tpad_scan(uint8_t mode)
{
    static uint8_t keyen = 0;   /* 0, ���Կ�ʼ���;  >0, �����ܿ�ʼ���; */
    uint8_t res = 0;
    uint8_t sample = 3;         /* Ĭ�ϲ�������Ϊ3�� */
    uint16_t rval;

    if (mode)
    {
        sample = 6;     /* ֧��������ʱ�����ò�������Ϊ6�� */
        keyen = 0;      /* ֧������, ÿ�ε��øú��������Լ�� */
    }

    rval = tpad_get_maxval(sample);

    if (rval > (g_tpad_default_val + TPAD_GATE_VAL))/* ����tpad_default_val+TPAD_GATE_VAL,��Ч */
    {
        if (keyen == 0)
        {
            res = 1;    /* keyen==0, ��Ч */
        }
        keyen = 3;      /* ����Ҫ�ٹ�3��֮����ܰ�����Ч */
    }

    if (keyen)keyen--;

    return res;
}

