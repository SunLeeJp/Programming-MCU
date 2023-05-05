
#include <avr/io.h>
#include "delay.h"
#include "LCD_4bits.h"
#include <stdio.h>
#include "DHT11.h"
#define BUZZER_PIN 2
int value_adc;
//int a = 0, b = 0;
float temp = 0;
float humi = 0;
int m,n;
int test = 65;
int clear_w = 0;
int clear_f = 0;
int Analog_Read1(char channel)
{
	int value;
	ADMUX |=  (1 << REFS0);  // Internal 2.56V Voltage Reference with external capacitor at AREF pin
	ADMUX |=  channel;  // chóse channel
	ADCSRA |= (1 << ADEN);  // set as 1 to enable ADC convert
	//ADCSRA |= (1 << ADATE); // covert negative egle
	ADCSRA |= (1 << ADSC);  // start convert
	while(1&(ADCSRA >> ADSC));   // wait when ADSC = 0;
	value = (ADCL) + (ADCH << 8);
	return value;
}

void Analog_Init()
{
	ADMUX |=  (1 << REFS0);  // Internal 2.56V Voltage Reference with external capacitor at AREF pin
	
	ADCSRA |= (1 << ADEN)|(1 << ADPS1)|(1 << ADPS0);  // set as 1 to enable ADC convert
}

int Analog_Read(char channel)
{
	ADMUX |=   channel | (1 << REFS0);  // chóse channel
	ADCSRA |= (1 << ADSC)|(1 << ADPS1)|(1 << ADPS0);  // start convert
	while(!(1 & (ADCSRA >> ADIF)));   // wating when flag adif set as 1
	//return ADCW;
	return (ADCL) + (ADCH << 8);
	
}



int main(void)
{
	DDRD = 0xFF;
	DDRC = 0xFF;
	DDRA |= 1 << BUZZER_PIN; 
//	PORTA |= 1 << BUZZER_PIN; 
	LCD_Init();
	LCD_Gotoxy(5,0);
	LCD_Puts("Welcome");
	LCD_Gotoxy(2,1);
	LCD_Puts("Enter reset ");
    while (1) 
    {
			DHT11_Read_Data(&humi, &temp);
			m = humi * 10;
			n = temp * 10;
			if(temp > 30)
			{
				if(clear_w == 0) 
				{
					LCD_Clear();
					clear_w = 1;
				}
				clear_f = 0;
				PORTA |= 1 << BUZZER_PIN;
//				DHT11_Read_Data(&humi, &temp);
				LCD_Gotoxy(3,0);
				LCD_Puts("Warning!!!");
				LCD_Gotoxy(0,1);
				LCD_Puts("TEMP: ");
				LCD_PutChar( n /100 + 48);
				LCD_PutChar((n%100)/10 + 48);
				LCD_PutChar('.');
				LCD_PutChar( n %10 + 48);
				LCD_Puts(" *C");
				//delay_ms(500);
			}
			else
			{
				if(clear_f == 0)
				{
					LCD_Clear();
					clear_f = 1;
				}
				clear_w = 0;
				LCD_Gotoxy(0,0);
				LCD_Puts("HUMI: ");
				LCD_PutChar( m /100 + 48);
				LCD_PutChar((m%100)/10 + 48);
				LCD_PutChar('.');
				LCD_PutChar( m %10 + 48);
				LCD_Puts(" %");
				LCD_Gotoxy(0,1);
				LCD_Puts("TEMP: ");
				LCD_PutChar( n /100 + 48);
				LCD_PutChar((n%100)/10 + 48);
				LCD_PutChar('.');
				LCD_PutChar( n %10 + 48);
				LCD_Puts(" *C");
				PORTA &= ~(1 << BUZZER_PIN);		
			}	
    }
}
