#include <stm32f10x_lib.h>
#include "sys.h"	
#include "delay.h"	
#include "stdio.h"
#include "init.h"	
 

u8 USART_RX_BUF[256];    //RXT buffer maxium 255bytes
u8 USART_RX_STA=0;       //RX Compleate	 
u8 USART_RX_CNT=0;       //RX Entre Count	
  	
int main(void)
{		
 	Stm32_Clock_Init(9); //system clock init   always init first
	IO_Init();
	delay_init(72);	     //delay init
	uart_init(72,9600);  //uart init
	PWM_Init(100,18000);	 //count to 100, freq/18000
//	LED0_PWM_VAL=led0pwmval;
	LED0_PWM_VAL=0x64;
   	while(1)
	{	
		USART1->DR=LED0_PWM_VAL;
		LED0=!LED0;
		while((USART1->SR&0X40)==0);//wait for the end of transmission	   
	}	 
} 

void USART1_IRQHandler(void)
{
	u8 res;	    
	if(USART1->SR&(1<<5))//RXT recieved
	{	 
		res=USART1->DR;	 //read out data and clear interrupt flag
		LED0_PWM_VAL=res;
		if(!USART_RX_STA)
		{ 	
			USART_RX_BUF[USART_RX_CNT++]=res;
			if(USART_RX_CNT==255)USART_RX_CNT=0;//RXT data over flow,start over	  
		}
	}  											 
}

void TIM4_IRQHandler(void)
{ 		    		  			    
	if(TIM4->SR&0X0001)//Overflow Interrupt
	{
		LED1=!LED1;			    				   				     	    	
	}				   
	TIM4->SR&=~(1<<0);//Clear Interrupt flag 	    
}