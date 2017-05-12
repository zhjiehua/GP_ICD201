#ifndef __EXTI_H
#define __EXIT_H	 
#include "sys.h"
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

//�����IC��ά���ź�����
#define WIEGAND26
//#define WIEGAND34

#ifdef WIEGAND26
#define PWMCNT (26-8)
#elif  WIEGAND34
#define PWMCNT 34
#endif

typedef enum
{
    TypeA = 0,
    TypeB,
}CardType_TypeDef;

typedef struct
{
    /*-------------------------ic reader-----------------------------------------*/ 
    u8 intFlag;  //�жϱ�־
    u8 intCnt;  //��¼��ǰIC����ȡ������������Ҫ���ڷ�ֹ����
    u8 timCnt;  //��ʱ���жϴ���
    
    u32 data; //����������
#ifdef WIEGANG34
    u8 data2;
#endif
    
    u8 pwmCnt;  //ic����Ӧ1�ε�������������ʼ����ֻ�����ں�intCnt�Ƚ�
    u8 timNum;  //ic����Ӧ1�ε�������ڵڼ�����ʱ�жϺ��ٳ��֣���ʼ����ֻ�����ں�timCnt�Ƚ�
    u8 icFlag;  //1����ʾ��ǰIC������

    /*--------------------------sensor-------------------------------------------*/    
    u8 sensorFlag; //�ӽ����ؼ�⵽�Ƿ��п���1���п�
    
    /*--------------------------card type----------------------------------------*/ 
    u16 timerCount;  //��ʱ���жϴ���
    u16 timerNumber; //��ʱ��
                    //���sensorFlag��ʾ�п���timerNumber�����ic����flag��Ϊ1����֤����B����A��������ΪA��
                    //���ߣ����icFlag��ʾic�����ڣ�timerNumber�����sensorFlagΪ1����ΪA�������򱨴�
    
    CardType_TypeDef cardType;//������
    
    /*--------------------------exception----------------------------------------*/
    
    u32 timExcpCnt;
    u32 timExcpNum;
    
    
}Card_TypeDef;

typedef struct
{
    u8 finish; //���һ�μ��
    
    u8 cardTypeBStat; //B��������ͳ�ƣ������ǰ��Ƭ��B�������1�������A���Ҵ���0�����1�����򲻱�
    
    CardType_TypeDef cardLine[4];
}CardMan_TypeDef;

extern Card_TypeDef *pCard;
extern CardMan_TypeDef *pcardMan;

void EXTIX_Init(void);//�ⲿ�жϳ�ʼ��		 					    
#endif

