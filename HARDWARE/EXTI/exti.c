#include "exti.h"
#include "led.h"
#include "key.h"
#include "delay.h"
#include "usart.h"
#include "beep.h"

//////////////////////////////////////////////////////////////////////////////////	 
//������ֻ��ѧϰʹ�ã�δ��������ɣ��������������κ���;
//ALIENTEKս��STM32������
//�ⲿ�ж� ��������	   
//����ԭ��@ALIENTEK
//������̳:www.openedv.com
//�޸�����:2012/9/3
//�汾��V1.0
//��Ȩ���У�����ؾ���
//Copyright(C) ������������ӿƼ����޹�˾ 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////   

Card_TypeDef card;
CardMan_TypeDef cardMan;

Card_TypeDef *pCard = &card;
CardMan_TypeDef *pcardMan = &cardMan;


//�ⲿ�ж�0�������
void EXTIX_Init(void)
{
 
 	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);//ʹ��PORTA,PORTEʱ��

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_13;//KEY0-KEY2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //���ó���������
 	GPIO_Init(GPIOC, &GPIO_InitStructure);//��ʼ��GPIOE2,3,4
    
  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//ʹ�ܸ��ù���ʱ��

    //�ⲿ�ж�����
    EXTI_ClearITPendingBit(EXTI_Line10 | EXTI_Line11 | EXTI_Line13);
    
    //GPIOE.2 �ж����Լ��жϳ�ʼ������   �½��ش���
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource13);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource10);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource11);
    
  	EXTI_InitStructure.EXTI_Line=EXTI_Line10 | EXTI_Line11 | EXTI_Line13;	//KEY2
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 	//����EXTI_InitStruct��ָ���Ĳ�����ʼ������EXTI�Ĵ���

  	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;			//ʹ�ܰ���WK_UP���ڵ��ⲿ�ж�ͨ��
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;	//��ռ���ȼ�2�� 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;					//�����ȼ�3
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//ʹ���ⲿ�ж�ͨ��
  	NVIC_Init(&NVIC_InitStructure); 
    
    //��ʼ���ṹ��
    //card
    card.intFlag = 0;
    card.intCnt = 0;
    card.timCnt = 0;
    card.icFlag = 0;
    card.pwmCnt = PWMCNT;
    card.timNum = 70;
    card.data = 0;
    
    card.sensorFlag = 0;
    
    card.timerCount = 0;
    card.timerNumber = 300;
    
    //cardMan
    cardMan.finish = 0;
    cardMan.cardTypeBStat = 0;
}

//�ⲿ�ж�0������� 
void EXTI15_10_IRQHandler(void)
{
	if(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_10))
    {
        pCard->intFlag = 1;
#ifdef WIEGANG34
        if(pCard->intCnt > 31)
        {
            pCard->data2 |= (0x0001<<pCard->intCnt-32);
        }
        else
#endif
        {
            pCard->data |= (0x0001<<pCard->intCnt);
        }
        pCard->intCnt++;
        
        EXTI_ClearITPendingBit(EXTI_Line10); //���LINE0�ϵ��жϱ�־λ 
    }
    
    if(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_11))
    {
        pCard->intFlag = 1;
#ifdef WIEGANG34
        if(pCard->intCnt > 31)
        {
            pCard->data2 &= ~(0x0001<<pCard->intCnt-32);
        }
        else
#endif
        {
            pCard->data &= ~(0x0001<<pCard->intCnt);
        }
        pCard->intCnt++;
        
        EXTI_ClearITPendingBit(EXTI_Line11); //���LINE0�ϵ��жϱ�־λ 
    }
    
    if(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13))
    {
        pCard->sensorFlag = 1;
        
        EXTI_ClearITPendingBit(EXTI_Line13); //���LINE0�ϵ��жϱ�־λ 
    }
	 
}
 
