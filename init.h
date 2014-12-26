#include "sys.h"
#define LED0_PWM_VAL TIM4->CCR2 
#define LED1 PAout(7)	// PA8
#define LED0 PBout(6)	// PA8

void IO_Init(void);	  	
void uart_init(u32 pclk2,u32 bound);
void Timerx_Init(u16 arr,u16 psc);
void PWM_Init(u16 arr,u16 psc);
