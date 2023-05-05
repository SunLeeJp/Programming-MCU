#include <avr/io.h>
#include "delay.h"
#include "LCD_4bits.h"
#include "DHT11.h"

#define DHT_PIN 1
#define DHT_DDR DDRA
#define DHT_OUTPUT PORTA
#define DHT_INPUT PINA

int x, y, z, t, c;
int a = 0, b = 0;
void DHT_GPIO_Write(uint8_t state)
{
	DHT_DDR |= (1 << DHT_PIN);
	if (state == 1)
	{
		DHT_OUTPUT |= (1 << DHT_PIN);
	}
	else DHT_OUTPUT &= ~(1 << DHT_PIN);
}
int DHT_GPIO_Read()
{
	DHT_DDR &= ~(1 << DHT_PIN);
	return 1&(DHT_INPUT >> DHT_PIN);
}

void DHT11_Init()
{
	DHT_GPIO_Write(1);
	DHT_GPIO_Write(0);
	delay_ms(20);
	DHT_GPIO_Write(1);
	//////////
	while(DHT_GPIO_Read() == 1);
	//delay_80us();
	while(DHT_GPIO_Read() == 0);
	//delay_50us();
	while(DHT_GPIO_Read() == 1);
	// start transmit data
}

int DHT11_Read()
{
	for (int i = 0; i < 8; i++)
	{
		while(DHT_GPIO_Read() == 0);
		delay_30us();
		if(DHT_GPIO_Read() == 1)  // muc high
		{
			a = (a << 1) | 0x01;
		}
		else a = (a << 1);
		while(DHT_GPIO_Read() == 1);
	}
	b = a;
	a = 0x00;
	return b;
}

void DHT11_Read_Data(float *humi, float *temp)
{
	DHT11_Init();
	x = DHT11_Read();
	y = DHT11_Read();
	z = DHT11_Read();
	t = DHT11_Read();
	c = DHT11_Read();
	*humi = x + (float)y/10.0;
	*temp = z + (float)t/10.0;
	delay_ms(100);
}
