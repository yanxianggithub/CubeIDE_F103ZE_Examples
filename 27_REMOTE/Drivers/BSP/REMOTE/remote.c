
#include "remote.h"
#include "../../SYSTEM/delay/delay.h"
#include "usart.h"
#include "tim.h"


/* ң��������״̬
 * [7]  : �յ����������־
 * [6]  : �õ���һ��������������Ϣ
 * [5]  : ����
 * [4]  : ����������Ƿ��Ѿ�������
 * [3:0]: �����ʱ��
 */
uint8_t g_remote_sta = 0;
uint32_t g_remote_data = 0; /* ������յ������� */
uint8_t  g_remote_cnt = 0;  /* �������µĴ��� */

/**
 * @brief       ��ʱ�������жϻص�����
 * @param       htim:��ʱ�����
 * @retval      ��
 */
void HAL_TIM_PeriodElapsedCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == REMOTE_IN_TIMX)
    {
        if (g_remote_sta & 0x80)      /* �ϴ������ݱ����յ��� */
        {
            g_remote_sta &= ~0X10;    /* ȡ���������Ѿ��������� */

            if ((g_remote_sta & 0X0F) == 0X00)
            {
                g_remote_sta |= 1 << 6; /* ����Ѿ����һ�ΰ����ļ�ֵ��Ϣ�ɼ� */
            }
            
            if ((g_remote_sta & 0X0F) < 14)
            {
                g_remote_sta++;
            }
            else
            {
                g_remote_sta &= ~(1 << 7);    /* ���������ʶ */
                g_remote_sta &= 0XF0;         /* ��ռ����� */
            }
        }
    }
}

/**
 * @brief       ��ʱ�����벶���жϻص�����
 * @param       htim:��ʱ�����
 * @retval      ��
 */
void HAL_TIM_IC_CaptureCallback(TIM_HandleTypeDef *htim)
{
    if (htim->Instance == REMOTE_IN_TIMX)
    {
        uint16_t dval;  /* �½���ʱ��������ֵ */
        
        if (RDATA)      /* �����ز��� */
        {
            __HAL_TIM_SET_CAPTUREPOLARITY(&htim4,REMOTE_IN_TIMX_CHY,TIM_INPUTCHANNELPOLARITY_FALLING);//CC4P=1 ����Ϊ�½��ز���
            __HAL_TIM_SET_COUNTER(&htim4, 0);  	/* ��ն�ʱ��ֵ */
            g_remote_sta |= 0X10;          		/* ����������Ѿ������� */
        }
        else           /* �½��ز��� */
        {
            dval=HAL_TIM_ReadCapturedValue(&htim4, REMOTE_IN_TIMX_CHY);                /* ��ȡCCR4Ҳ������CC4IF��־λ */
            __HAL_TIM_SET_CAPTUREPOLARITY(&htim4, REMOTE_IN_TIMX_CHY, TIM_INPUTCHANNELPOLARITY_RISING);/* ����TIM4ͨ��4�����ز��� */

            if (g_remote_sta & 0X10)        /* ���һ�θߵ�ƽ���� */
            {
                if (g_remote_sta & 0X80)    /* ���յ��������� */
                {

                    if (dval > 300 && dval < 800)           /* 560Ϊ��׼ֵ,560us */
                    {
                        g_remote_data >>= 1;                /* ����һλ */
                        g_remote_data &= ~(0x80000000);     /* ���յ�0 */
                    }
                    else if (dval > 1400 && dval < 1800)    /* 1680Ϊ��׼ֵ,1680us */
                    {
                        g_remote_data >>= 1;                /* ����һλ */
                        g_remote_data |= 0x80000000;        /* ���յ�1 */
                    }
                    else if (dval > 2000 && dval < 3000)    /* �õ�������ֵ���ӵ���Ϣ 2500Ϊ��׼ֵ2.5ms */
                    {
                        g_remote_cnt++;         /* ������������1�� */
                        g_remote_sta &= 0XF0;   /* ��ռ�ʱ�� */
                    }
                }
                else if (dval > 4200 && dval < 4700)    /* 4500Ϊ��׼ֵ4.5ms */
                {
                    g_remote_sta |= 1 << 7; /* ��ǳɹ����յ��������� */
                    g_remote_cnt = 0;       /* ����������������� */
                }
            }

            g_remote_sta&=~(1<<4);
        }
    }
}

/**
 * @brief       ������ⰴ��(���ư���ɨ��)
 * @param       ��
 * @retval      0   , û���κΰ�������
 *              ����, ���µİ�����ֵ
 */
uint8_t remote_scan(void)
{
    uint8_t sta = 0;
    uint8_t t1, t2;

    if (g_remote_sta & (1 << 6))    /* �õ�һ��������������Ϣ�� */
    {
        t1 = g_remote_data;                 /* �õ���ַ�� */
        t2 = (g_remote_data >> 8) & 0xff;   /* �õ���ַ���� */

        if ((t1 == (uint8_t)~t2) && t1 == REMOTE_ID)    /* ����ң��ʶ����(ID)����ַ */
        {
            t1 = (g_remote_data >> 16) & 0xff;
            t2 = (g_remote_data >> 24) & 0xff;

            if (t1 == (uint8_t)~t2)
            {
                sta = t1;           /* ��ֵ��ȷ */
            }
        }

        if ((sta == 0) || ((g_remote_sta & 0X80) == 0)) /* �������ݴ���/ң���Ѿ�û�а����� */
        {
            g_remote_sta &= ~(1 << 6);  /* ������յ���Ч������ʶ */
            g_remote_cnt = 0;           /* ����������������� */
        }
    }

    return sta;
}











