#ifndef __LED_H
#define	__LED_H

#include "stm32f1xx.h"

//���Ŷ���
/*******************************************************/
#define LED_PIN                  GPIO_PIN_13                
#define LED_GPIO_PORT            GPIOC                     
#define LED_GPIO_CLK_ENABLE()   __HAL_RCC_GPIOC_CLK_ENABLE()


/************************************************************/


/** ����LED������ĺ꣬
	* LED�͵�ƽ��������ON=0��OFF=1
	* ��LED�ߵ�ƽ�����Ѻ����ó�ON=1 ��OFF=0 ����
	*/
#define ON  GPIO_PIN_RESET
#define OFF GPIO_PIN_SET

/* ���κ꣬��������������һ��ʹ�� */
#define LED(a)	HAL_GPIO_WritePin(LED_GPIO_PORT,LED_PIN,a)


/* ֱ�Ӳ����Ĵ����ķ�������IO */
#define	digitalHi(p,i)			{p->BSRR=i;}			  //����Ϊ�ߵ�ƽ		
#define digitalLo(p,i)			{p->BSRR=(uint32_t)i << 16;}				//����͵�ƽ
#define digitalToggle(p,i)		{p->ODR ^=i;}			//�����ת״̬


/* �������IO�ĺ� */
#define LED_TOGGLE		digitalToggle(LED_GPIO_PORT,LED_PIN)
#define LED_OFF				digitalHi(LED_GPIO_PORT,LED_PIN)
#define LED_ON				digitalLo(LED_GPIO_PORT,LED_PIN)

void LED_GPIO_Config(void);

#endif /* __LED_H */
