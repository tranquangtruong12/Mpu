#include "I2C.h"


void I2C_init(void)
{
	//enable clock I2C
	RCC->APB1ENR |= RCC_APB1ENR_I2C1EN;
	//resert sofware
	I2C1->CR1 |= 0x8000;
	I2C1->CR1 &= ~0x8000; 
	/*Program the periphearal input clocl in I2C_CR2
	register in order to genarate correct timings*/
	I2C1->CR2 |= 36u;//freq is 36Mhz
	//config the clock control registers
	//(fPCLK1 = 36MHz, VDD_I2C = 3.3V)
	//RP = External pull-up reistance, fSCL = I2C speed
	//RP = 4.7 kO, Rs: resistor protect bus I2C
	I2C1->CCR = 180u; //0xB4 = 100khz : Tan so: SCL
	//config the rise time register
	I2C1->TRISE = 37u;
	//Program the I2C_CR1 register to enable the peripheral
	I2C1->CR1 |= (uint16_t)0x01;
	
	//enable ACK
	I2C1->CR1 |= (uint32_t)0x01 <<10u; //use for receive more bytes
}

void sendData(uint8_t I2C_Address, uint8_t address, uint8_t value)
{
	//make sure Busy is clear
	while((I2C1->SR2) & 0x02){}
	//set the start bit in the I2c_CR1
	I2C1->CR1 |= 0x100; //bit 8(start)
		
		
		//EV5: SB=1, clear by reading SR1 rg, write DR register with Add 
	while(!(I2C1->SR1 & 0x01)){}
	//delay(2u);
	I2C1->DR = I2C_Address <<1u; 
		
		
	//EV6n ADDR =1 
	while(!(I2C1->SR1 & 0x02)){}
	I2C1->SR2;
		
		//EV8_1: TxE =1, shirf register empty
	while(!(I2C1->SR1 & 0x80)){} //xac dinh thanh ghi empty 
	I2C1->DR = address;
		
	//EV8 Txe =1, shirf register not empty
	while(!(I2C1->SR1 & 0x80)){}
	I2C1->DR = value;
		//EV8_2: TxE=1: thanh ghi empty , BTF = 1: truyen thanh cong;
	while(!(I2C1->SR1 & 0x80 && I2C1->SR1 & 0x04)){} 
	//set stop bit
	I2C1->CR1 |= 0x200;
}
void ReceiveOneByte(uint8_t I2C_Address, uint8_t address, uint8_t *data)
{
	//make sure Busy is clear
	while((I2C1->SR2)&0x02){}
	//set the start bit in the i2c_cr1 register to generate a start condition
	I2C1->CR1 |= 0x100;
		//EV5: SB = 1, cleared by reading SR1 register followed by writing DR
	while(!(I2C1->SR1 & 0x01)){}
	//delay(10u);
	I2C1->DR = I2C_Address <<1u;
	
		
	//EV6: ADDR =1
	while(!(I2C1->SR1 & 0x02)){}
	I2C1->SR2;
	
	//EV8_1: TXE = 1
	//Write register address of mpu in DR register
	while(!(I2C1->SR1 & 0x08)){}
	I2C1->DR = address;
	while(!(I2C1->SR1 & 0x80)){}

	//read data
		
	/*Set the start bit */
	I2C1->CR1 |= 0x100;
		//EV5:SB=1
	while(!(I2C1->SR1 & 0x01)){}
	I2C1->DR = (I2C_Address << 1u) | 0x01;
	
		/*EV6_3: ADDR = 1, ACK = 0;*/
	while(!(I2C1->SR1 & 0x02)){}
	I2C1->CR1 &= ~0x40; //clear ACK bit
	I2C1->SR2;   				//read sr2 to clear ADDR
	I2C1->CR1 |= 0x200; //set stop bit
		
	//EV7
	while(!(I2C1->SR1 & 0x40)){}
	*data = I2C1->DR;
}
