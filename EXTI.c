#include "EXTI.h"

void EXT0_config(void)
{
	//enable AFIO
	RCC->APB2ENR |= 0x01;
	//config pin interrupts
	AFIO->EXTICR[0] = 0x01;
	//resert pending bit
	EXTI->PR |= 0x01;
	//select falling edge
	EXTI->FTSR |= 0x01;
	//disable rising edge
	EXTI->RTSR &= ~(0x01);
	//interrupts mask register
	EXTI->IMR |= 0x01;
	//disable event mask register
	EXTI->EMR = 0x00;;
	//NVIC
	NVIC->ISER[0] = 0x40;
}
