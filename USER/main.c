#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "exti.h"
#include "beep.h"
#include "timer.h"
 
/************************************************
 ALIENTEK战舰STM32开发板实验5
 外部中断实验   
 技术支持：www.openedv.com
 淘宝店铺：http://eboard.taobao.com 
 关注微信公众平台微信号："正点原子"，免费获取STM32资料。
 广州市星翼电子科技有限公司  
 作者：正点原子 @ALIENTEK
************************************************/

 int main(void)
 {		
 
	delay_init();	    	 //延时函数初始化	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //设置NVIC中断分组2:2位抢占优先级，2位响应优先级
	uart_init(115200);	 //串口初始化为115200
 	LED_Init();		  		//初始化与LED连接的硬件接口
	BEEP_Init();         	//初始化蜂鸣器端口
	EXTIX_Init();		 	//外部中断初始化
    TIM3_Int_Init(9,7199);//10Khz的计数频率，计数到10为1ms
	LED0=0;					//点亮LED0
    LED1=1;
	while(1)
	{	    
//        if(pCard->icFlag)
//        {
//            pCard->icFlag = 0;
//            printf("IC-card-low-data:0x%x!\r\n", (int)pCard->data);
//            printf("IC-card-high-data:0x%x!\r\n", (int)(pCard->data>>16));
//            pCard->data = 0;
//            LED0=!LED0;
//        }
        if(pcardMan->finish)
        {
            pcardMan->finish = 0;
            
            if(pCard->cardType == TypeA)
            {
                if(pcardMan->cardTypeBStat > 0)
                {
                    if(pcardMan->cardTypeBStat >= 2)//如果连续出现了B卡，现在又出现了A卡，则B卡的统计变量重新计算
                        pcardMan->cardTypeBStat = 0;
                    else
                        pcardMan->cardTypeBStat--;
                }
                LED0 = !LED0;
            }
            else if(pCard->cardType == TypeB)
            {
                pcardMan->cardTypeBStat++;
                LED1 = !LED1;
            }
            
            if(pcardMan->cardTypeBStat >= 2)
            {
                if(pcardMan->cardTypeBStat >= 6)//如果连续6张都是B卡，则表示进入了全B卡模式
                    pcardMan->cardTypeBStat--;
                
                if(pcardMan->cardTypeBStat%2 == 0)//剔除第2或第4张B卡，因为这2张非常有可能是坏的A卡
                {
                    //剔除当前卡片
                    BEEP = 1;
                    delay_ms(300);
                    BEEP = 0;
                    //printf("get rid of current card\n");
                }
            }
        }
	}
 }

