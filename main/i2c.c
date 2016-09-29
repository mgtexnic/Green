

#include <avr/io.h>

#include "header/i2c.h"
#include "header/var1.h"



void start()
{
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWEN);					// ������������ ��������� "�����"
	while (!(TWCR & (1<<TWINT)));							// ��������� ���� ��� TWINT � �������� TWCR �������
}
void restart()												// ������������ ��������� "��������"
{
	start();
}
void stop()
{
	TWCR = (1<<TWINT)|(1<<TWEN)|(1<<TWSTO);					// ������������ ��������� "����"
}
void stop_start()											// ������������ ��������� "����-�����"
{
	TWCR = (1<<TWINT)|(1<<TWSTA)|(1<<TWSTO)|(1<<TWEN);		// ������������ ��������� "�����"
	while (!(TWCR & (1<<TWINT)));							// ��������� ���� ��� TWINT � �������� TWCR �������
}
void byte_write(unsigned char data)
{
	TWDR = data;							
	TWCR = (1<<TWINT)|(1<<TWEN);
	while (!(TWCR & (1<<TWINT)));							// ��������� ���� ��� TWINT � �������� TWCR �������
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
	while (!(TWCR & (1<<TWINT)));				// ��������� ���� ��� TWINT � �������� TWCR �������
	data = TWDR;
	return data;								// ������ ������ �� �������� DS1307 (��������� �������)
}
void Transmit_DS1307(unsigned char data, unsigned char reg)
{
	unsigned char address_w;
	address_w = 0b11010000;
	
	start();								// ������������ ��������� "�����"
	byte_write(address_w);					// ������ ������ DS1307 (0b11010000)									
	byte_write(reg);						// ����� �������� ��� ������ DS1307
	byte_write(data);						// ������ ������
	stop();									// ������������ ��������� "����"
}
unsigned char Receive_DS1307(unsigned char reg)
{
	unsigned char data;
	unsigned char address_w;
	unsigned char address_r; 
	address_w = 0b11010000;
	address_r = 0b11010001;
	
	start();								// ������������ ��������� "�����"
	byte_write(address_w);					// ������ ������ DS1307 (0b11010000)
	byte_write(reg);
	start();								// ����� �������� ��� ������ DS1307
	byte_write(address_r);					// ������ ������ DS1307 (0b11010001)
	data = byte_read(0x00);
	stop();									// ������������ ��������� "����"
	return data;
	
}
void TWI_Transmit(unsigned char address, unsigned char data)
{
	start();								// ������������ ��������� "�����"
	byte_write(address);					// ������ ������ DS1307 (0b11010000)
	byte_write(data);						// ������ ������
	stop();									// ������������ ��������� "����"
}