/*
 * DHT11.h
 *
 * Created: 12/19/2022 10:02:10 PM
 *  Author: huyle
 */ 


#ifndef DHT11_H_
#define DHT11_H_

void DHT_GPIO_Write(uint8_t state);
int DHT_GPIO_Read();
void DHT11_Init();
int DHT11_Read();
void DHT11_Read_Data(float *humi, float *temp);



#endif /* DHT11_H_ */