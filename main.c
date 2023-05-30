#include "GPIO.h"
#include "I2C.h"
#include "EXTI.h"
#include <math.h>
#include "stm32f10x.h"

#define sample_rate 35
#define config 			26
#define gyro_config 27
#define acc_config  28
#define Level_INT   55
#define interrupts  56
#define pwr_manager 107
#define acc_x 			59
#define acc_y				61
#define acc_z 			63

/* init value */
volatile float ax,ay,az,pitch,roll;
//get value acc
uint16_t MeasureAcc(uint8_t address);

int main(void)
{
	GPIO_I2C_INIT();
	I2C_init();
	sendData(0x68/*dia chi mpu*/, sample_rate/*dia chi thanh ghi*/, 19u/*gia tri thanh ghi*/);
	sendData(0x68, config, 5u);
	sendData(0x68, gyro_config, 8u);
	sendData(0x68, acc_config, 0x10);
	sendData(0x68, Level_INT,0x80);
	sendData(0x68, interrupts, 1u);
	sendData(0x68, pwr_manager, 1u);
	GPIO_EXTI_config();
	EXT0_config();
	while(1){}	
}
/*..................subrountine............*/
uint16_t MeasureAcc(uint8_t address)
{
	uint8_t value[2];
	uint16_t raw;
	ReceiveOneByte(0x68, address, value);//nhan 8 bit cao
	ReceiveOneByte(0x68, address+1, value+1);//nhan 8 bit thap
	raw = (uint16_t)value[0]<<8 | value[1];
	return raw;
}

void EXTI0_IRQHandler(void) // ham ngat
{
	if(EXTI->PR & 0x01 && EXTI->IMR & 0x01)
	{
		ax = (float)MeasureAcc(59)*0.0078;
		ay = (float)MeasureAcc(61)*0.0078;
		az = (float)MeasureAcc(63)*0.0078;
		pitch = atan2(ax, sqrt(pow(ay,2)+pow(az,2)))*180/(atan(1)*4);
		//roll = atan2(-ay,az)*180.0/(atan(1)*4);
		EXTI->PR |= 0x01;
	}
}