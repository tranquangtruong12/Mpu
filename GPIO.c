#include "GPIO.h"

void GPIO_I2C_INIT(void)
{
	//enable clock port B
	RCC->APB2ENR |= (uint32_t)0x01 << 3u;
	//enable altermate clock
	RCC->APB2ENR |= (uint32_t)0x01 << 1u;
	//config PB6 as SCL pin and PB7 as SDA pin; alternate open drain
	GPIOB->CRL &= ~((uint32_t)0xFF << 24u);
	GPIOB->CRL |= (uint32_t)0xFF << 24u;//11: output 50khz, 11: Alternate function output Open-drain
	//PuLl up
	//GPIO->ODR = (uint32_t)0x03<<6u;
}


void GPIO_EXTI_config(void)
{
	//config pin b0
	GPIO_InitTypeDef GPIO_InitStruct;
	RCC->APB2ENR |= 0x08;
	GPIO_InitStruct.GPIO_Pin = GPIO_Pin_0;
	GPIO_InitStruct.GPIO_Speed = GPIO_Speed_50MHz;
	GPIO_InitStruct.GPIO_Mode = GPIO_Mode_IPU;
	GPIO_Init(GPIOB, &GPIO_InitStruct);
}