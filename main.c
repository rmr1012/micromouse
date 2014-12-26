#include <stm32f10x_lib.h>
#include "sys.h"	
#include "delay.h"	
#include "stdio.h"
#include "init.h"	
 

u8 USART_RX_BUF[256];    //RXT buffer maxium 255bytes
u8 USART_RX_STA=0;       //RX Compleate	 
u8 USART_RX_CNT=0;       //RX Entre Count	
u16 echo=0x7855;
u8 echoc;  
char pussy='b';	
int main(void)
{		
 	Stm32_Clock_Init(9); //system clock init   always init first
	IO_Init();
	GPIOB->CRL&=0XF0FFFFFF;//PB6 Output
	GPIOB->CRL|=0X03000000;//AOUT 
	delay_init(72);	     //delay init
	uart_init(72,9600);  //uart init
	PWM_Init(100,18000);	 //count to 100, freq/18000
	Timer3_Init(65535,30);
	EXTIX_Init();
//	LED0_PWM_VAL=led0pwmval;
	LED0_PWM_VAL=0x64;
	
   	while(1)
	{	
		printf("%u",echo);
		printf("\n");
		while((USART1->SR&0X40)==0);//wait for transfer to compleate
		LED0=!LED0;
	
		delay_ms(100);//wait for the end of transmission	   
	}	 
} 
void EXTI0_IRQHandler(void)
{
	delay_us(10);
	if(PBin(0)==1)
	{
		TIM3->CR1|=0x01;   //start timer
	}
	else
	{
		TIM3->CR1&=0xfe;//stop timer
		echo=TIM3->CNT;	//read value
		TIM3->CNT=0x00; //clear timer
		pussy='a';
	}
	EXTI->PR=1<<0;  //清除LINE0上的中断标志位  
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