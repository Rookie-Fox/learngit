#include "stm32f1xx_hal.h"
#include "lcd.h"
#include "stdio.h"
#include "string.h"

#define RS_GPIO_Port GPIOA
#define RW_GPIO_Port GPIOA
#define EN_GPIO_Port GPIOA

#define RS_Pin GPIO_PIN_1
#define RW_Pin GPIO_PIN_2
#define EN_Pin GPIO_PIN_3

#define RS_HIGH HAL_GPIO_WritePin(RS_GPIO_Port,RS_Pin,GPIO_PIN_SET)
#define RS_LOW HAL_GPIO_WritePin(RS_GPIO_Port,RS_Pin,GPIO_PIN_RESET)
#define RW_HIGH HAL_GPIO_WritePin(RW_GPIO_Port,RW_Pin,GPIO_PIN_SET)
#define RW_LOW HAL_GPIO_WritePin(RW_GPIO_Port,RW_Pin,GPIO_PIN_RESET)
#define EN_HIGH HAL_GPIO_WritePin(EN_GPIO_Port,EN_Pin,GPIO_PIN_SET)
#define EN_LOW HAL_GPIO_WritePin(EN_GPIO_Port,EN_Pin,GPIO_PIN_RESET)

uint8_t ReadData(void)
{
    uint8_t dat=0;
    if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_0)==GPIO_PIN_SET) dat|=0x01;
    if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_1)==GPIO_PIN_SET) dat|=0x02;
    if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_2)==GPIO_PIN_SET) dat|=0x04;
    if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_3)==GPIO_PIN_SET) dat|=0x08;
    if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_4)==GPIO_PIN_SET) dat|=0x10;
    if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_5)==GPIO_PIN_SET) dat|=0x20;
    if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_6)==GPIO_PIN_SET) dat|=0x40;
    if(HAL_GPIO_ReadPin(GPIOB, GPIO_PIN_7)==GPIO_PIN_SET) dat|=0x80;
    return dat;
}

unsigned char ReadStatusLCD(void)
{
    uint8_t LCD_Data = 0xFF;
    
    RS_LOW;    // ����RSΪ�͵�ƽ����ʾ��������
    RW_HIGH;   // ����RWΪ�ߵ�ƽ����ʾ��ȡ����
    EN_LOW;    // ʹ�ܶ��õ͵�ƽ

    HAL_Delay(1); // �ȴ�һ��ʱ�䣬ȷ�������ȶ�

    EN_HIGH;   // ʹ�ܶ��øߵ�ƽ�������ȡ����
    HAL_Delay(1); // �ȴ�һ��ʱ�䣬ȷ�������ȶ�
    
    // ��ȡæ״̬λ
    LCD_Data = ReadData();

    EN_LOW;    // ʹ�ܶ��õ͵�ƽ��������ȡ

    return(LCD_Data);
}

void Write_Cmd_Func(uint8_t cmd)
{
    while (ReadStatusLCD() & 0x80); // �ȴ�LCD��æ
    RS_LOW;
    RW_LOW;
    EN_LOW;
    GPIOB->ODR = cmd;
    HAL_Delay(5);
    EN_HIGH;
    HAL_Delay(5);
    EN_LOW;
}

void Write_Data_Func(uint8_t data)
{
    while (ReadStatusLCD() & 0x80); // �ȴ�LCD��æ
    RS_HIGH;
    RW_LOW;
    EN_LOW;
    GPIOB->ODR = data;
    HAL_Delay(5);
    EN_HIGH;
    HAL_Delay(5);
    EN_LOW;
}

void LCD_Init(void)
{
    while (ReadStatusLCD() & 0x80); // �ȴ�LCD��æ
    HAL_Delay(15);
    Write_Cmd_Func(0x38);
    HAL_Delay(5);
    Write_Cmd_Func(0x38);//дָ�� 38H����ʾģʽ����
    Write_Cmd_Func(0x08);//дָ�� 08H����ʾ�ر�
    Write_Cmd_Func(0x01);//дָ�� 01H����ʾ����
    Write_Cmd_Func(0x06);//дָ�� 06H����ʾ����ƶ�����
    Write_Cmd_Func(0x0c);//дָ�� 0CH����ʾ�����������}
}

void LCD_ShowLine(uint8_t row, uint8_t col, char *string)
{
    while (ReadStatusLCD() & 0x80); // �ȴ�LCD��æ
    switch(row)
    {
        case 1:
            Write_Cmd_Func(0x80+col);
        while(*string)
        {
            Write_Data_Func(*string);
            string++;
        }
            break;
        
        case 2:
            Write_Cmd_Func(0x80+0x40+col);
        while(*string)
        {
            Write_Data_Func(*string);
            string++;
        }
            break;
    }
}

void LCD_ShowNumber(uint8_t row, uint8_t col, int num)
{
    char buffer[16];
    
    snprintf(buffer, sizeof(buffer), "%d", num); // ������ת��Ϊ�ַ���
    LCD_ShowLine(row, col, buffer); // ������ʾ�ַ����ĺ�����ʾ����
}
while(1){};
this is test1

