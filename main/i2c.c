

#include <avr/io.h>

#include "header/i2c.h"
#include "header/var1.h"



void start()
{
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);					// Формирование состояния "Старт"
	while (!(TWCR & (1<<TWINT)));							// Выполнять пока бит TWINT в регистре TWCR сброшен
}
void restart()												// Формирование состояния "ПовСтарт"
{
	start();
}
void stop()
{
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);					// Формирование состояния "Стоп"
}
void stop_start()											// Формирование состояния "Стоп-Старт"
{
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWSTO)|(1<<TWEN);		// Формирование состояния "Старт"
	while (!(TWCR & (1<<TWINT)));							// Выполнять пока бит TWINT в регистре TWCR сброшен
}
void byte_write(unsigned char data)
{
	TWDR = data;							
	TWCR = (1<<TWINT)|(1<<TWEN);
	while (!(TWCR & (1<<TWINT)));							// Выполнять пока бит TWINT в регистре TWCR сброшен
}
unsigned char byte_read(unsigned char ask)
{
	unsigned char data;
	
	if (ask)
	{
		TWCR |= (1<<TWEA);
	} 
	else
	{
		TWCR &= ~(1<<TWEA);
	}	
	TWCR |= (1<<TWINT);
	while (!(TWCR & (1<<TWINT)));				// Выполнять пока бит TWINT в регистре TWCR сброшен
	data = TWDR;
	return data;								// Чтение данных из регистра DS1307 (выбранный регистр)
}
void Transmit_DS1307(unsigned char data, unsigned char reg)
{
	unsigned char address_w;
	address_w = 0b11010000;
	
	start();								// Формирование состояния "Старт"
	byte_write(address_w);					// Запись адреса DS1307 (0b11010000)									
	byte_write(reg);						// Выбор регистра для записи DS1307
	byte_write(data);						// Запись данных
	stop();									// Формирование состояния "Стоп"
}
unsigned char Receive_DS1307(unsigned char reg)
{
	unsigned char data;
	unsigned char address_w;
	unsigned char address_r; 
	address_w = 0b11010000;
	address_r = 0b11010001;
	
	start();								// Формирование состояния "Старт"
	byte_write(address_w);					// Запись адреса DS1307 (0b11010000)
	byte_write(reg);
	start();								// Выбор регистра для записи DS1307
	byte_write(address_r);					// Запись адреса DS1307 (0b11010001)
	data = byte_read(0x00);
	stop();									// Формирование состояния "Стоп"
	return data;
	
}
void TWI_Transmit(unsigned char address, unsigned char data)
{
	start();								// Формирование состояния "Старт"
	byte_write(address);					// Запись адреса DS1307 (0b11010000)
	byte_write(data);						// Запись данных
	stop();									// Формирование состояния "Стоп"
}