/* Includes ------------------------------------------------------------------*/
#include "main.h"
#include <stdio.h>
#define ARRAYSIZE 8
#define ADC1_DR    ((uint32_t)0x4001244C)
volatile uint16_t ADC_values[ARRAYSIZE];
volatile uint16_t PTchl[8];
volatile uint16_t PTchh[8];
volatile uint32_t status = 0;
__IO uint16_t IC2Value = 0;
__IO uint16_t DutyCycle = 0;
__IO uint32_t Frequency = 0;
__IO uint16_t IC2Value1 = 0;
__IO uint16_t DutyCycle1 = 0;
__IO uint32_t Frequency1 = 0;
__IO uint32_t RT_Count=0;
__IO uint32_t fuckme=0;
//uint32_t TIM3Freq = 0;
 
void ADCInit(void);
void DMAInit(void);
void RCC_Configuration(void);
void Delay(__IO uint32_t nCount);
void GPIO_Configuration(void);
void Usart1Init(void);
void Usart1Put(uint8_t);
void TIM_Config(void);
uint8_t Usart1Get(void);

#define LED PCout(13)
#define mux PCout(15)

int main(void)
{
	//uint8_t index;
	RCC_Configuration();
	GPIO_Configuration();
	Usart1Init();
	ADCInit();
	DMAInit();
	TIM_Config();
	SysTick_Config(SystemCoreClock/10000);
	
	DMA_Cmd(DMA1_Channel1, ENABLE);
	//Start ADC1 Software Conversion

	//print averages
	/*for(index = 0; index<8; index++)
			{
			printf("ch%d = %d ",index, ADC_values[index]);
			}*/
			ADC_SoftwareStartConvCmd(ADC1, ENABLE);
	while (1)
	{
	
		/* Insert delay */

//		printf("fuck you");

	//wait for DMA complete
	//	Delay(10);
			fuckme=mux;
			printf("%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\t%d\r\n",RT_Count,fuckme,Frequency,Frequency1,PTchl[0],PTchl[1],PTchl[2],PTchl[3],PTchl[4],PTchl[5],PTchl[6],PTchl[7],PTchh[0],PTchh[1],PTchh[2],PTchh[3],PTchh[4],PTchh[5],PTchh[6],PTchh[7]);
	//	GPIO_ResetBits(GPIOC, GPIO_Pin_13);
	}
}

 
void Delay(__IO uint32_t num)
{
	__IO uint32_t index = 0;

	/* default system clock is 72MHz */
	for(index = (720 * num); index != 0; index--)
	{
	}
}

void RCC_Configuration(void)
{
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOC, ENABLE);
}

void GPIO_Configuration(void)
{
	GPIO_InitTypeDef GPIO_InitStructure;
	
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_13|GPIO_Pin_15;
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_Out_PP;
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	
	GPIO_Init(GPIOC, &GPIO_InitStructure);
}

#ifdef  USE_FULL_ASSERT
/**
  * @brief  Reports the name of the source file and the source line number
  *         where the assert_param error has occurred.
  * @param  file: pointer to the source file name
  * @param  line: assert_param error line source number
  * @retval None
  */
void assert_failed(uint8_t* file, uint32_t line)
{ 
  /* User can add his own implementation to report the file name and line number,
     ex: printf("Wrong parameters value: file %s on line %d\r\n", file, line) */

  /* Infinite loop */
  while (1)
  {
  }
}
#endif

void Usart1Init(void) 
{
 
	GPIO_InitTypeDef GPIO_InitStructure;
	 
	USART_InitTypeDef USART_InitStructure;
	 
	USART_ClockInitTypeDef USART_ClockInitStructure;
	 
	//enable bus clocks
	 
	RCC_APB2PeriphClockCmd(RCC_APB2Periph_USART1 | RCC_APB2Periph_GPIOA | RCC_APB2Periph_AFIO, ENABLE);
	 
	//Set USART1 Tx (PA.09) as AF push-pull
	 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_9;
	 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AF_PP;
	 
	GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;
	 
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	 
	//Set USART1 Rx (PA.10) as input floating
	 
	GPIO_InitStructure.GPIO_Pin = GPIO_Pin_10;
	 
	GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
	 
	GPIO_Init(GPIOA, &GPIO_InitStructure);
	 
	USART_ClockStructInit(&USART_ClockInitStructure);
	 
	USART_ClockInit(USART1, &USART_ClockInitStructure);
	 
	USART_InitStructure.USART_BaudRate = 256000;
	 
	USART_InitStructure.USART_WordLength = USART_WordLength_9b;
	 
	USART_InitStructure.USART_StopBits = USART_StopBits_1;
	 
	USART_InitStructure.USART_Parity = USART_Parity_Even ;
	 
	USART_InitStructure.USART_Mode = USART_Mode_Rx | USART_Mode_Tx;
	 
	USART_InitStructure.USART_HardwareFlowControl = USART_HardwareFlowControl_None;
	 
	//Write USART1 parameters
	 
	USART_Init(USART1, &USART_InitStructure);
	 
	//Enable USART1
	 
	USART_Cmd(USART1, ENABLE);
	USART_ClockStructInit(&USART_ClockInitStructure);
	USART_ClockInit(USART1, &USART_ClockInitStructure);
 
}

void ADCInit(void){
		ADC_InitTypeDef ADC_InitStructure;
    //ADC1 configuration
    GPIO_InitTypeDef GPIO_InitStructure; //Variable used to setup the GPIO pins
    //==Configure ADC pins (PA0 -> Channel 0 to PA7 -> Channel 7) as analog inputs==
	  //--Enable ADC1 and GPIOA--
    RCC_APB2PeriphClockCmd(RCC_APB2Periph_ADC1 | RCC_APB2Periph_GPIOA, ENABLE);
    GPIO_StructInit(&GPIO_InitStructure); // Reset init structure, if not it can cause issues...
    GPIO_InitStructure.GPIO_Pin = GPIO_Pin_0 | GPIO_Pin_1| GPIO_Pin_2| GPIO_Pin_3| GPIO_Pin_4| GPIO_Pin_5| GPIO_Pin_6| GPIO_Pin_7;
    GPIO_InitStructure.GPIO_Mode = GPIO_Mode_AIN;
    GPIO_Init(GPIOA, &GPIO_InitStructure);

    RCC_ADCCLKConfig(RCC_PCLK2_Div6);
 	
    ADC_InitStructure.ADC_Mode = ADC_Mode_Independent;
    //We will convert multiple channels
    ADC_InitStructure.ADC_ScanConvMode = ENABLE;
    //select continuous conversion mode
    ADC_InitStructure.ADC_ContinuousConvMode = ENABLE;//!
    //select no external triggering
    ADC_InitStructure.ADC_ExternalTrigConv = ADC_ExternalTrigConv_None;
    //right 12-bit data alignment in ADC data register
    ADC_InitStructure.ADC_DataAlign = ADC_DataAlign_Right;
    //8 channels conversion
    ADC_InitStructure.ADC_NbrOfChannel = 8;
    //load structure values to control and status registers
    ADC_Init(ADC1, &ADC_InitStructure);
    //wake up temperature sensor
    //ADC_TempSensorVrefintCmd(ENABLE);
    //configure each channel
    ADC_RegularChannelConfig(ADC1, ADC_Channel_0, 1, ADC_SampleTime_7Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_1, 2, ADC_SampleTime_7Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_2, 3, ADC_SampleTime_7Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_3, 4, ADC_SampleTime_7Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_4, 5, ADC_SampleTime_7Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_6, 6, ADC_SampleTime_7Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_5, 7, ADC_SampleTime_7Cycles5);
    ADC_RegularChannelConfig(ADC1, ADC_Channel_7, 8, ADC_SampleTime_7Cycles5);
    //Enable ADC1
    ADC_Cmd(ADC1, ENABLE);
    //enable DMA for ADC
    ADC_DMACmd(ADC1, ENABLE);
    //Enable ADC1 reset calibration register
    ADC_ResetCalibration(ADC1);
    //Check the end of ADC1 reset calibration register
    while(ADC_GetResetCalibrationStatus(ADC1));
    //Start ADC1 calibration
    ADC_StartCalibration(ADC1);
    //Check the end of ADC1 calibration
    while(ADC_GetCalibrationStatus(ADC1));
}
void DMAInit(void){
	//create DMA structure
    DMA_InitTypeDef  DMA_InitStructure;
	  NVIC_InitTypeDef NVIC_InitStructure;
    //Enable DMA1 channel IRQ Channel */
    //enable DMA1 clock
    RCC_AHBPeriphClockCmd(RCC_AHBPeriph_DMA1, ENABLE);
    //reset DMA1 channe1 to default values;
    DMA_DeInit(DMA1_Channel1);
    //channel will be used for memory to memory transfer
    DMA_InitStructure.DMA_M2M = DMA_M2M_Disable;
    //setting normal mode (non circular)
    DMA_InitStructure.DMA_Mode = DMA_Mode_Circular;
    //medium priority
    DMA_InitStructure.DMA_Priority = DMA_Priority_High;
    //source and destination data size word=32bit
    DMA_InitStructure.DMA_PeripheralDataSize = DMA_PeripheralDataSize_HalfWord;
    DMA_InitStructure.DMA_MemoryDataSize = DMA_MemoryDataSize_HalfWord;
    //automatic memory destination increment enable.
    DMA_InitStructure.DMA_MemoryInc = DMA_MemoryInc_Enable;
    //source address increment disable
    DMA_InitStructure.DMA_PeripheralInc = DMA_PeripheralInc_Disable;
    //Location assigned to peripheral register will be source
    DMA_InitStructure.DMA_DIR = DMA_DIR_PeripheralSRC;
    //chunk of data to be transfered
    DMA_InitStructure.DMA_BufferSize = ARRAYSIZE;
    //source and destination start addresses
    DMA_InitStructure.DMA_PeripheralBaseAddr = (uint32_t)ADC1_DR;
    DMA_InitStructure.DMA_MemoryBaseAddr = (uint32_t)ADC_values;
    //send values to DMA registers
    DMA_Init(DMA1_Channel1, &DMA_InitStructure);
    // Enable DMA1 Channel Transfer Complete interrupt
    DMA_ITConfig(DMA1_Channel1, DMA_IT_TC, ENABLE);
    DMA_Cmd(DMA1_Channel1, ENABLE); //Enable the DMA1 - Channel1

    NVIC_InitStructure.NVIC_IRQChannel = DMA1_Channel1_IRQn;
    NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelSubPriority = 0;
    NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
    NVIC_Init(&NVIC_InitStructure);
 
 
}
void TIM_Config(void)
{
  GPIO_InitTypeDef GPIO_InitStructure;
  NVIC_InitTypeDef NVIC_InitStructure;
	TIM_ICInitTypeDef  TIM_ICInitStructure;

//	GPIO_InitTypeDef GPIO_InitStructure1;
  NVIC_InitTypeDef NVIC_InitStructure1;
	TIM_ICInitTypeDef  TIM_ICInitStructure1;
	
  RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM4, ENABLE);
	RCC_APB1PeriphClockCmd(RCC_APB1Periph_TIM2, ENABLE);

  /* GPIOA clock enable */
  RCC_APB2PeriphClockCmd(RCC_APB2Periph_GPIOB, ENABLE);


  /* NVIC configuration */
//  NVIC_InitTypeDef NVIC_InitStructure;

  /* Enable the TIM4 global Interrupt */
  NVIC_InitStructure.NVIC_IRQChannel = TIM4_IRQn;
  NVIC_InitStructure.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure.NVIC_IRQChannelSubPriority = 1;
  NVIC_InitStructure.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure);
	NVIC_InitStructure1.NVIC_IRQChannel = TIM2_IRQn;
  NVIC_InitStructure1.NVIC_IRQChannelPreemptionPriority = 0;
  NVIC_InitStructure1.NVIC_IRQChannelSubPriority = 2;
  NVIC_InitStructure1.NVIC_IRQChannelCmd = ENABLE;
  NVIC_Init(&NVIC_InitStructure1);


	
  /* Configure the GPIO ports */


  /* TIM4 channel 2 pin (PA.07) configuration */
  GPIO_InitStructure.GPIO_Pin = GPIO_Pin_7|GPIO_Pin_3;
  GPIO_InitStructure.GPIO_Mode = GPIO_Mode_IN_FLOATING;
  GPIO_InitStructure.GPIO_Speed = GPIO_Speed_50MHz;

  GPIO_Init(GPIOB, &GPIO_InitStructure);



  /* TIM4 configuration: PWM Input mode ------------------------
     The external signal is connected to TIM4 CH2 pin (PA.01), 
     The Rising edge is used as active edge,
     The TIM4 CCR2 is used to compute the frequency value 
     The TIM4 CCR1 is used to compute the duty cycle value
  ------------------------------------------------------------ */

  TIM_ICInitStructure.TIM_Channel = TIM_Channel_2;
  TIM_ICInitStructure.TIM_ICPolarity = TIM_ICPolarity_Rising;
  TIM_ICInitStructure.TIM_ICSelection = TIM_ICSelection_DirectTI;
  TIM_ICInitStructure.TIM_ICPrescaler = TIM_ICPSC_DIV1;
  TIM_ICInitStructure.TIM_ICFilter = 0x0;
	
  TIM_PWMIConfig(TIM4, &TIM_ICInitStructure);
	TIM_PWMIConfig(TIM2, &TIM_ICInitStructure);

  /* Select the TIM4 Input Trigger: TI2FP2 */
  TIM_SelectInputTrigger(TIM4, TIM_TS_TI2FP2);
  TIM_SelectInputTrigger(TIM2, TIM_TS_TI2FP2);
  /* Select the slave Mode: Reset Mode */
  TIM_SelectSlaveMode(TIM4, TIM_SlaveMode_Reset);
  TIM_SelectSlaveMode(TIM2, TIM_SlaveMode_Reset);
  /* Enable the Master/Slave Mode */
  TIM_SelectMasterSlaveMode(TIM4, TIM_MasterSlaveMode_Enable);
  TIM_SelectMasterSlaveMode(TIM2, TIM_MasterSlaveMode_Enable);
  /* TIM enable counter */
  TIM_Cmd(TIM2, ENABLE);
  TIM_Cmd(TIM4, ENABLE);
  /* Enable the CC2 Interrupt Request */
  TIM_ITConfig(TIM4, TIM_IT_CC2, ENABLE);
	TIM_ITConfig(TIM2, TIM_IT_CC2, ENABLE); 
}
/*******************************************************************************
* Function Name  	: DMA1_Channel1_IRQHandler
* Return         	: None
* Parameters 		: None
* Created by		: Phung The Vu
* Date created	: 19/01/2009
* Description		: This function handles DMA1 Channel 1 interrupt request.
* Notes			: 
*******************************************************************************/
void DMA1_Channel1_IRQHandler(void)
{
//Test on DMA1 Channel1 Transfer Complete interrupt
	static uint16_t cnt=0;
	uint8_t i=0;	
  if(DMA_GetITStatus(DMA1_IT_TC1))
  {
	  status=1;
		LED=!LED;
   //Clear DMA1 interrupt pending bits
    DMA_ClearITPendingBit(DMA1_IT_GL1);
		cnt++;
		if(cnt==10){
			mux=!mux;
			cnt=0;
		}
		if(mux){
			for(i=0;i<=ARRAYSIZE;i++)
				PTchl[i]=ADC_values[i];
		}
		else{
			for(i=0;i<=ARRAYSIZE;i++)
				PTchh[i]=ADC_values[i];
		}	
  }
}
void TIM4_IRQHandler(void)
{
  /* Clear TIM4 Capture compare interrupt pending bit */
  TIM_ClearITPendingBit(TIM4, TIM_IT_CC2);

  /* Get the Input Capture value */
  IC2Value = TIM_GetCapture2(TIM4);

  if (IC2Value != 0)
  {
    /* Duty cycle computation */
    DutyCycle = (TIM_GetCapture1(TIM4) * 100) / IC2Value;

    /* Frequency computation */
    Frequency = SystemCoreClock / IC2Value;
//		printf("%d/n",Frequency);
  }
  else
  {
    DutyCycle = 0;
    Frequency = 0;
  }
}
void TIM2_IRQHandler(void)
{
  /* Clear TIM4 Capture compare interrupt pending bit */
  TIM_ClearITPendingBit(TIM2, TIM_IT_CC2);

  /* Get the Input Capture value */
  IC2Value1 = TIM_GetCapture2(TIM2);

  if (IC2Value1 != 0)
  {
    /* Duty cycle computation */
    DutyCycle1 = (TIM_GetCapture1(TIM2) * 100) / IC2Value1;

    /* Frequency computation */
    Frequency1 = SystemCoreClock / IC2Value1;
//		printf("%d/n",Frequency);
  }
  else
  {
    DutyCycle1 = 0;
    Frequency1 = 0;
  }
}
/**
  * @brief  This function handles SysTick Handler.
  * @param  None
  * @retval None
  */
void SysTick_Handler(void)
{
	RT_Count++;
}

/*

void __fat_print_char(char c) {
	f_write(&FATFS_logfile,&c,(UINT)1,&a);
}

/**
  * @brief  Writes a char to string - use for better logfile performance
  * @param  Character to write
  * @retval None
  *//*
void __str_print_char(char c) {
	uint8_t indx=strlen(print_string)%255;		//Make sure we cant overwrite ram
	print_string[indx]=c;				//Append string
	print_string[indx+1]=0x00;			//Null terminate
	__usart_send_char(c);				//Send to the bluetooth as well
}*/