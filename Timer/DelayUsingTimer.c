#include "RccConfig.h"


void TIM6Config (void)
{

// 1. Enable Timer clock
	RCC->APB1ENR |= (1<<4);  // Enable the timer6 clock
	
// 2. Set the prescalar and the ARR
	TIM6->PSC = 90-1;  // 90MHz/90 = 1 MHz ~~ 1 uS delay
	TIM6->ARR = 0xffff;  // MAX ARR value
	
// 3. Enable the Timer, and wait for the update Flag to set
	TIM6->CR1 |= (1<<0); // Enable the Counter
	while (!(TIM6->SR & (1<<0)));  // UIF: Update interrupt flag..  This bit is set by hardware when the registers are updated
}

void Delay_us (uint16_t us)
{
	
	// RESET the Counter
	// Wait for the Counter to reach the entered value. As each count will take 1 us, 
	// the total waiting time will be the required us delay
	
	TIM6->CNT = 0;
	while (TIM6->CNT < us);
}

void Delay_ms (uint16_t ms)
{
	for (uint16_t i=0; i<ms; i++)
	{
		Delay_us (1000); // delay of 1 ms
	}
}


void GPIOConfig (void)
{
	
	// Enable GPIOA clock
	RCC->AHB1ENR |= (1<<0);
	
	// Set the PIN PA5 as output
	GPIOA->MODER |= (1<<10);
	
}

	


int main (void)
{
	SysClockConfig ();
	TIM6Config ();
	GPIOConfig ();
		
	while (1)
	{
		GPIOA->BSRR |= (1<<5); // Enable Pin PA5
		Delay_ms (1000);
		GPIOA->BSRR |= (1<<5) <<16;  // Disable Pin PA5
		Delay_ms (1000);
	}
	
}

