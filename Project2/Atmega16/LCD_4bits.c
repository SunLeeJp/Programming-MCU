#define F_CPU 8000000
#include <avr/io.h>
#include "delay.h"
#include "LCD_4bits.h"
#define LCD_RS 6
#define LCD_RW 5
#define LCD_EN 7
#define LCD_D4 4           //Truyen du lieu qua 4 chan
#define LCD_D5 5
#define LCD_D6 6
#define LCD_D7 7
#define DATA PORTC
#define CONTROL PORTD




/*Chuong trình giao tiep LCD 16x2 4bit*/
void LCD_Enable(void)
{
	CONTROL |= (1 << LCD_EN);  //LCD_EN = 1;
	delay_7us();
	CONTROL &= ~(1 << LCD_EN);  //LCD_EN=0;
	delay_50us();
}
void LCD_Send4Bit(unsigned char Data)
{
	//LCD_D4=Data & 0x01;
	if (Data & 0x01)
	{
		DATA |= (1 << LCD_D4);
	}
	else DATA &= ~(1 << LCD_D4);
	
	//LCD_D5=(Data>>1)&1;
	if ((Data >> 1) & 1)
	{
		DATA |= (1 << LCD_D5);
	}
	else DATA &= ~(1 << LCD_D5);
	//LCD_D6=(Data>>2)&1;
	if ((Data >> 2) & 1)
	{
		DATA |= (1 << LCD_D6);
	}
	else DATA &= ~(1 << LCD_D6);
	//LCD_D7=(Data>>3)&1;
	if ((Data >> 3) & 1)
	{
		DATA |= (1 << LCD_D7);
	}
	else DATA &= ~(1 << LCD_D7);
}
void LCD_SendCommand(unsigned char command)
{
	LCD_Send4Bit(command >>4);
	LCD_Enable();
	LCD_Send4Bit(command);
	LCD_Enable();
}
void LCD_Clear()
{
	LCD_SendCommand(0x01);
	delay_ms(2);
}
void LCD_Init()
{
	LCD_Send4Bit(0x00);
	delay_ms(20);
	CONTROL &= ~(1 << LCD_RS);//LCD_RS=0;
	CONTROL &= ~(1 << LCD_RW);//LCD_RW=0;
	LCD_Send4Bit(0x03);
	LCD_Enable();
	delay_ms(5);
	LCD_Enable();
	delay_100us(100);
	LCD_Enable();
	LCD_Send4Bit(0x02);
	LCD_Enable();
	LCD_SendCommand( 0x28 );
	LCD_SendCommand( 0x0C);
	LCD_SendCommand( 0x06 );
	LCD_SendCommand(0x01);
	LCD_SendCommand( 0x02 );
	delay_ms(2);
}
void LCD_Gotoxy(int x, int y)
{
	unsigned char address;
	if(!y) address=(0x80+x);
	else address=(0xc0+x);
	delay_ms(1);
	LCD_SendCommand(address);
	delay_50us();


}
void LCD_PutChar(unsigned char Data)
{
	CONTROL |= (1 << LCD_RS);//LCD_RS=1;
	LCD_SendCommand(Data);
	CONTROL &= ~(1 << LCD_RS);//LCD_RS=0 ;
}
void LCD_Puts(char *s)
{
	while (*s)
	{
		LCD_PutChar(*s);
		s++;
	}
}

void num_dsp( float number)
{
	//	unsigned char str[5];
	//	sprintf(str,"%3.0f",number);
	//	LCD_Puts(str);
}

