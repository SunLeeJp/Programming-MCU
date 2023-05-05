/*
 * LCD_4bits.h
 *
 * Created: 12/16/2022 3:12:44 PM
 *  Author: huyle
 */ 


#ifndef LCD_4BITS_H_
#define LCD_4BITS_H_
void LCD_Enable(void);
void LCD_Send4Bit(unsigned char Data);
void LCD_SendCommand(unsigned char command);
void LCD_Clear();
void LCD_Init();
void LCD_Gotoxy(int x, int y);
void LCD_PutChar(unsigned char Data);
void LCD_Puts(char *s);
void num_dsp( float number);





#endif /* LCD_4BITS_H_ */