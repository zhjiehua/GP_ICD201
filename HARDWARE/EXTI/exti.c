#include "exti.h"
#include "led.h"
#include "key.h"
#include "delay.h"
#include "usart.h"
#include "beep.h"

//////////////////////////////////////////////////////////////////////////////////	 
//本程序只供学习使用，未经作者许可，不得用于其它任何用途
//ALIENTEK战舰STM32开发板
//外部中断 驱动代码	   
//正点原子@ALIENTEK
//技术论坛:www.openedv.com
//修改日期:2012/9/3
//版本：V1.0
//版权所有，盗版必究。
//Copyright(C) 广州市星翼电子科技有限公司 2009-2019
//All rights reserved									  
//////////////////////////////////////////////////////////////////////////////////   

Card_TypeDef card;
CardMan_TypeDef cardMan;

Card_TypeDef *pCard = &card;
CardMan_TypeDef *pcardMan = &cardMan;


//外部中断0服务程序
void EXTIX_Init(void)
{
 
 	EXTI_InitTypeDef EXTI_InitStructure;
 	NVIC_InitTypeDef NVIC_InitStructure;
    GPIO_InitTypeDef GPIO_InitStructure;
 
 	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC,ENABLE);//使能PORTA,PORTE时钟

	GPIO_InitStructure.GPIO_Pin  = GPIO_Pin_10|GPIO_Pin_11|GPIO_Pin_13;//KEY0-KEY2
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IPU; //设置成上拉输入
 	GPIO_Init(GPIOC, &GPIO_InitStructure);//初始化GPIOE2,3,4
    
  	RCC_APB2PeriphClockCmd(RCC_APB2Periph_AFIO,ENABLE);	//使能复用功能时钟

    //外部中断配置
    EXTI_ClearITPendingBit(EXTI_Line10 | EXTI_Line11 | EXTI_Line13);
    
    //GPIOE.2 中断线以及中断初始化配置   下降沿触发
  	GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource13);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource10);
    GPIO_EXTILineConfig(GPIO_PortSourceGPIOC,GPIO_PinSource11);
    
  	EXTI_InitStructure.EXTI_Line=EXTI_Line10 | EXTI_Line11 | EXTI_Line13;	//KEY2
  	EXTI_InitStructure.EXTI_Mode = EXTI_Mode_Interrupt;	
  	EXTI_InitStructure.EXTI_Trigger = EXTI_Trigger_Falling;
  	EXTI_InitStructure.EXTI_LineCmd = ENABLE;
  	EXTI_Init(&EXTI_InitStructure);	 	//根据EXTI_InitStruct中指定的参数初始化外设EXTI寄存器

  	NVIC_InitStructure.NVIC_IRQChannel = EXTI15_10_IRQn;			//使能按键WK_UP所在的外部中断通道
  	NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0x01;	//抢占优先级2， 
  	NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0x03;					//子优先级3
  	NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;								//使能外部中断通道
  	NVIC_Init(&NVIC_InitStructure); 
    
    //初始化结构体
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

//外部中断0服务程序 
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
        
        EXTI_ClearITPendingBit(EXTI_Line10); //清除LINE0上的中断标志位 
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
        
        EXTI_ClearITPendingBit(EXTI_Line11); //清除LINE0上的中断标志位 
    }
    
    if(!GPIO_ReadInputDataBit(GPIOC, GPIO_Pin_13))
    {
        pCard->sensorFlag = 1;
        
        EXTI_ClearITPendingBit(EXTI_Line13); //清除LINE0上的中断标志位 
    }
	 
}
 
