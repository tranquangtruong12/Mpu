#include "stm32f10x.h"
void I2C_init(void);
void sendData(uint8_t I2C_address, uint8_t address, uint8_t value);
void ReceiveOneByte(uint8_t I2C_Address, uint8_t address, uint8_t *data);

