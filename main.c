#include <stm32f10x_lib.h>
#include "sys.h"	
#include "delay.h"	
#include "stdio.h"
#include "init.h"	
 

u8 USART_RX_BUF[256];    //RXT buffer maxium 255bytes
u8 USART_RX_STA=0;       //RX Compleate	 
u8 USART_RX_CNT=0;       //RX Entre Count	
u16 echo=0x7855;
u16 adcx1,adcx2,adcx3,adcx4,Gyro_raw,Batt; 
u8 cntt=1;
char pussy='b';	
int c,d,e,f;
char mem=1;
int main(void)
{		
 	Stm32_Clock_Init(9); //system clock init   always init first
	IO_Init();
	delay_init(72);	     //delay init
	uart_init(72,115200);  //uart init
	PWM4_Init(150,18000);				 // ultra ping sig
//	PWM2_1Init(800,18000);	 //count to 150, freq/18000	// ultra ping sig  .does not work
	PWM2_2Init(100,1800);	 //count to 100, freq/1800	 //IR aqusition pulse
	Timer3_Init(65535,256);	  //time refrence for echo/ultrasonic
	EXTIX_Init();
	Adc_Init();
	LED0_PWM_VAL2_2=0x44;
//	LED0_PWM_VAL2_1=0x34;
	LED0_PWM_VAL4=0x34;
	IRO=0;//turn on IR LEDs

   	while(1)
	{	
		printf("adcx1\tadcx2\tadcx3\tadcx4\tGyro\tBatt\techo\n");
		printf("%u\t%u\t%u\t%u\t%u\t%u\t%u",adcx1,adcx2,adcx3,adcx4,Gyro_raw,Batt,echo);
		printf("\n");
	//	USART1->DR=echo/0xff;
	//	while((USART1->SR&0X40)==0);//wait for transfer to compleate
		LED0=!LED0; 
		delay_us(100000);//wait for the end of transmission	   
	}	 
} 

void EXTI0_IRQHandler(void)
{
	EXTI->PR=1<<0;  //Clear Interrupt flag
//	delay_us(50);
	if(PBin(0)==1&&mem==0)
	{
		TIM3->CR1|=0x01;   //start timer
		mem=1;
	}
	else if(PBin(0)==0&&mem==1)
	{
		TIM3->CR1&=0xfe;//stop timer
		echo=TIM3->CNT;	//read value
		TIM3->CNT=0x00; //clear timer
		mem=0;
	} 
}

void USART1_IRQHandler(void)
{
	u8 res;	    
	if(USART1->SR&(1<<5))//RXT recieved
	{	 
		res=USART1->DR;	 //read out data and clear interrupt flag
//		LED0_PWM_VAL=res;
		if(!USART_RX_STA)
		{ 	
			USART_RX_BUF[USART_RX_CNT++]=res;
			if(USART_RX_CNT==255)USART_RX_CNT=0;//RXT data over flow,start over	  
		}
	}  											 
}

void TIM2_IRQHandler(void)	 
{ 		    		  			    
	if(TIM2->SR&0X0001)//Overflow Interrupt
	{
//		IRO=0;//defult 1
				    				   				     	    	
	}				   
	TIM2->SR&=~(1<<0);//Clear Interrupt flag 	    
}

void ADC1_2_IRQHandler(void)	 
{ 		    		  			    
	switch (cntt){
		case 1:
			adcx1=ADC1->DR;
			ADC1->DR=0;
			break;
		case 2:
			adcx2=ADC1->DR;
			ADC1->DR=0;
		 	break;
		case 3:
			adcx3=ADC1->DR;
			ADC1->DR=0;
			break;
		case 4:
			adcx4=ADC1->DR;
			ADC1->DR=0;
			break;
		case 5:
			Gyro_raw=ADC1->DR;
			ADC1->DR=0;
			break;
		case 6:
			Batt=ADC1->DR;
			ADC1->DR=0;
			break;
	}
	cntt++;
	if(cntt==7)
	{
		cntt=1;
	//	IRO=1;
	}
//	IRO=!IRO;//defult 1	    				   				     	    	   
}


