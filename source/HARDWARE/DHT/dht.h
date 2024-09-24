#ifndef __DHT_H
#define __DHT_H

#include "stm32f1xx_hal.h"

void delay_us(uint32_t us);
void DHT_GPIO_Init(uint32_t Mode);
void DHT11_Start();
uint8_t* DHT11_Read();

#endif
