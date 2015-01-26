#include "sys.h"
#define LED0_PWM_VAL4 TIM4->CCR2
#define LED0_PWM_VAL2_1 TIM2->CCR1
#define LED0_PWM_VAL2_2 TIM2->CCR2 
#define IRO PBout(5) 
//Human-Interface-input
#define BUT1 PBin(12) 
#define BUT2 PBin(11)
//Mot2 Commands
#define CDDECAY PBout(13)
#define CDRESETN PBout(9)
#define CDUSM1 PBout(8)
#define CDDIR PBout(6)
#define CDUSM0 PBout(5)
#define CDSTEP PBout(4)
//Mot1 Commands
#define ABDECAY PBout(14)	
#define ABRESTN PBout(3)
#define ABUSM1 PAout(15)
#define ABDIR PAout(12)
#define ABUSM0 PAout(11)
#define ABSTEP PBout(15)
//Human-Interface-output
#define WARNING PBout(1)
#define LED0 PCout(13)
#define LED1 PBout(14)
//Gyro Commands
#define G_ST PCout(15)
#define G_SLP PAout(8)

void Adc_Init(void);
u16  Get_Adc(u8 ch); 
void IO_Init(void);
void Motors_Init(u16 USM,u16 DECAY);	  	
void uart_init(u32 pclk2,u32 bound);
void Timer2_Init(u16 arr,u16 psc);
void Timer3_Init(u16 arr,u16 psc);
void Timer4_Init(u16 arr,u16 psc);
void PWM2_1Init(u16 arr,u16 psc);
void PWM2_2Init(u16 arr,u16 psc);
void PWM4_Init(u16 arr,u16 psc);
void EXTIX_Init(void);
