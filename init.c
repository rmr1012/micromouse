#include "init.h"
void SystemInit(void);
void IO_Init()
{
	RCC->APB2ENR|=1<<3;    //Enable PortB Clock	  
	RCC->APB2ENR|=1<<2;    //Enable PortA Clock	  	 
}

void PWM_Init(u16 arr,u16 psc)
{		 					 
	RCC->APB1ENR|=1<<2;    //TIM4 Clock Enable    

	GPIOA->CRH&=0XFFFFFFF0;//PA8 Input
	GPIOA->CRH|=0X00000004;//float HR
	  	
	GPIOB->CRL&=0X0FFFFFFF;//PA7 Output
	GPIOB->CRL|=0XB0000000;//AOUT 	  
	GPIOB->ODR|=1<<7;//PA7 Pull up	

	TIM4->ARR=arr;//Auto Reload Value 
	TIM4->PSC=psc;//Freq Devider
	
	TIM4->CCMR1|=7<<12;  //CH2 PWM2 mode		 
	TIM4->CCMR1|=1<<11; //CH2preload enable   

	TIM4->CCER|=1<<4;   //OC2 output enable	   

	TIM4->CR1=0x80;     //ARPE enable
	TIM4->CR1|=0x01;    //enable timer3 									  
}  	 
void Timerx_Init(u16 arr,u16 psc)
{
	RCC->APB1ENR|=1<<2;//TIM4 clock enable    
 	TIM4->ARR=arr;  //auto reload value 
	TIM4->PSC=psc;  //freq devider

	TIM4->DIER|=1<<0;   //allow update interrupt				
	TIM4->DIER|=1<<6;   //allow trigger interrupt	   
	TIM4->CR1|=0x01;    //enable Timer 4
  	MY_NVIC_Init(1,3,TIM4_IRQChannel,2);//Req1£¬SubPrioity3£¬Group2									 
}
void uart_init(u32 pclk2,u32 bound)
{  	 
	float temp;
	u16 mantissa;
	u16 fraction;	   
	temp=(float)(pclk2*1000000)/(bound*16);//get USARTDIV
	mantissa=temp;				 //get natrual part
	fraction=(temp-mantissa)*16; //get decimal part
    mantissa<<=4;
	mantissa+=fraction; 
	RCC->APB2ENR|=1<<2;   //enable PortA clock  
	RCC->APB2ENR|=1<<14;  //enable USART clock 
	GPIOA->CRH&=0XFFFFF00F; 
	GPIOA->CRH|=0X000008B0;//IO status
		  
	RCC->APB2RSTR|=1<<14;   //reset usart1
	RCC->APB2RSTR&=~(1<<14);//stop reset	   	   

 	USART1->BRR=mantissa; // Baud Rate config	 
	USART1->CR1|=0X200C;  //1 stop bit, no check.


	USART1->CR1|=1<<8;    //PE interrupt enable
	USART1->CR1|=1<<5;    //RXT buffer non-empty interrupt enable	    	
	MY_NVIC_Init(3,3,USART1_IRQChannel,2);//group2£¬low priority 

}
