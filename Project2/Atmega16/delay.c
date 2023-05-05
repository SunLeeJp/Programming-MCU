#include <avr/io.h>
#include "delay.h"

void delay_ms(unsigned long t)
{
	
	while(t --)
	{
		int i = 37;
		while(i--)
		{
			TCNT2 = 0x38;
			TCCR2 |= (1 << CS20);
			while ((TIFR & 0b01000000) == 0);
			TCCR2 &= ~(1 << CS20);
			TIFR = 0b01000000;
		}
	}
}

void delay_us(unsigned long t)
{
	TCNT1 = 65536 - t * 8;
	TCCR1B = (1 << CS10);    // non prescale
	while((1&(TIFR >> TOV1)) == 1);
	TCCR1B = 0x00;
	TIFR |= (1 << TOV1);    // set flag TOV1 as 1  
	
}
void delay_7us()
{
	TCNT2 = 0xFE;
	TCCR2 |= (1 << CS20);
	while ((TIFR & 0b01000000) == 0);
	TCCR2 &= ~(1 << CS20);
	TIFR = 0b01000000;
}

void delay_10us()
{
	TCNT2 = 0xE5;
	TCCR2 |= (1 << CS20);
	while ((TIFR & 0b01000000) == 0);
	TCCR2 &= ~(1 << CS20);
	TIFR = 0b01000000;
}
void delay_50us()
{
	for(int i = 0; i < 6; i++)
	{
		
		delay_10us();
	}
	delay_7us();
	
}
void delay_100us()
{
	delay_50us();
	delay_50us();
}

void delay_30us()
{
	delay_10us();
	delay_10us();
	delay_10us();
	delay_10us();
}

void delay_80us()
{
	delay_50us();
	delay_30us();
	delay_10us();
}