#include "sys.h"
#define LED0_PWM_VAL4 TIM4->CCR2
#define LED0_PWM_VAL2_1 TIM2->CCR1
#define LED0_PWM_VAL2_2 TIM2->CCR2  
#define LED1 PAout(7)	// PA8
#define LED0 PBout(6)	// PA8
#define IRO PBout(5)	// PA8
   
void Adc_Init(void);
u16  Get_Adc(u8 ch); 
void IO_Init(void);	  	
void uart_init(u32 pclk2,u32 bound);
void Timer2_Init(u16 arr,u16 psc);
void Timer3_Init(u16 arr,u16 psc);
void Timer4_Init(u16 arr,u16 psc);
void PWM2_1Init(u16 arr,u16 psc);
void PWM2_2Init(u16 arr,u16 psc);
void PWM4_Init(u16 arr,u16 psc);
void EXTIX_Init(void);
