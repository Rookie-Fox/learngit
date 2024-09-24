#ifndef __LCD_H
#define __LCD_H
#include "stm32f1xx_hal.h"

void Write_Cmd_Func(uint8_t cmd);
void Write_Data_Func(uint8_t data);
void LCD_Init(void);
void LCD_ShowLine(uint8_t row, uint8_t col, char *string);
uint8_t ReadData(void);
unsigned char ReadStatusLCD(void);
void LCD_ShowNumber(uint8_t row, uint8_t col, int num);

#endif
