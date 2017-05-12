#include "led.h"
#include "delay.h"
#include "key.h"
#include "sys.h"
#include "usart.h"
#include "exti.h"
#include "beep.h"
#include "timer.h"
 
/************************************************
 ALIENTEKս��STM32������ʵ��5
 �ⲿ�ж�ʵ��   
 ����֧�֣�www.openedv.com
 �Ա����̣�http://eboard.taobao.com 
 ��ע΢�Ź���ƽ̨΢�źţ�"����ԭ��"����ѻ�ȡSTM32���ϡ�
 ������������ӿƼ����޹�˾  
 ���ߣ�����ԭ�� @ALIENTEK
************************************************/

 int main(void)
 {		
 
	delay_init();	    	 //��ʱ������ʼ��	  
	NVIC_PriorityGroupConfig(NVIC_PriorityGroup_2); //����NVIC�жϷ���2:2λ��ռ���ȼ���2λ��Ӧ���ȼ�
	uart_init(115200);	 //���ڳ�ʼ��Ϊ115200
 	LED_Init();		  		//��ʼ����LED���ӵ�Ӳ���ӿ�
	BEEP_Init();         	//��ʼ���������˿�
	EXTIX_Init();		 	//�ⲿ�жϳ�ʼ��
    TIM3_Int_Init(9,7199);//10Khz�ļ���Ƶ�ʣ�������10Ϊ1ms
	LED0=0;					//����LED0
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
                    if(pcardMan->cardTypeBStat >= 2)//�������������B���������ֳ�����A������B����ͳ�Ʊ������¼���
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
                if(pcardMan->cardTypeBStat >= 6)//�������6�Ŷ���B�������ʾ������ȫB��ģʽ
                    pcardMan->cardTypeBStat--;
                
                if(pcardMan->cardTypeBStat%2 == 0)//�޳���2���4��B������Ϊ��2�ŷǳ��п����ǻ���A��
                {
                    //�޳���ǰ��Ƭ
                    BEEP = 1;
                    delay_ms(300);
                    BEEP = 0;
                    //printf("get rid of current card\n");
                }
            }
        }
	}
 }

