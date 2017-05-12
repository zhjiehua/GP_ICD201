#ifndef __EXTI_H
#define __EXIT_H	 
#include "sys.h"
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

//定义读IC卡维根信号类型
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
    u8 intFlag;  //中断标志
    u8 intCnt;  //记录当前IC卡读取的脉冲数，主要用于防止误判
    u8 timCnt;  //定时器中断次数
    
    u32 data; //读到的数据
#ifdef WIEGANG34
    u8 data2;
#endif
    
    u8 pwmCnt;  //ic卡感应1次的总脉冲数，初始化后只是用于和intCnt比较
    u8 timNum;  //ic卡感应1次的脉冲会在第几个定时中断后不再出现，初始化后只是用于和timCnt比较
    u8 icFlag;  //1：表示当前IC卡存在

    /*--------------------------sensor-------------------------------------------*/    
    u8 sensorFlag; //接近开关检测到是否有卡，1：有卡
    
    /*--------------------------card type----------------------------------------*/ 
    u16 timerCount;  //定时器中断次数
    u16 timerNumber; //定时器
                    //如果sensorFlag显示有卡，timerNumber后，如果ic卡的flag不为1，则证明是B卡或坏A卡，否则为A卡
                    //或者，如果icFlag显示ic卡存在，timerNumber后，如果sensorFlag为1，则为A卡，否则报错
    
    CardType_TypeDef cardType;//卡类型
    
    /*--------------------------exception----------------------------------------*/
    
    u32 timExcpCnt;
    u32 timExcpNum;
    
    
}Card_TypeDef;

typedef struct
{
    u8 finish; //完成一次检测
    
    u8 cardTypeBStat; //B卡连续性统计，如果当前卡片是B卡，则加1，如果是A卡且大于0，则减1，否则不变
    
    CardType_TypeDef cardLine[4];
}CardMan_TypeDef;

extern Card_TypeDef *pCard;
extern CardMan_TypeDef *pcardMan;

void EXTIX_Init(void);//外部中断初始化		 					    
#endif

