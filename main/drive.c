
#include <avr/io.h>
#define F_CPU 20000000		//Частота кварцевого резонатора
#include <util/delay.h>
#include "header/io.h"
#include "header/var1.h"
#include "header/lcd.h"
#include "header/i2c.h"
#include "header/drive.h"


//Управление электроприводом двери
unsigned char power_on_door(unsigned char power)
{
	unsigned char address;
	address = 0b01001110;
	power |=(1<<3);
	start();
	byte_write(address);
	byte_write(power);
	stop();
	return power;
}
unsigned char power_off_door(unsigned char power)
{
	unsigned char address;
	address = 0b01001110;
	power &=~(1<<3);
	start();
	byte_write(address);
	byte_write(power);
	stop();
	return power;
}
unsigned char open_door(unsigned char power)
{
	unsigned char address;
	address = 0b01001110;
	power &=~(1<<2);
	start();
	byte_write(address);
	byte_write(power);
	stop();
	return power;
}
unsigned char close_door(unsigned char power)
{
	unsigned char address;
	address = 0b01001110;
	power |=(1<<2);
	start();
	byte_write(address);
	byte_write(power);
	stop();
	return power;
}
//Управление электроприводом крана
unsigned char power_on_kran(unsigned char power)
{
	unsigned char address;
	address = 0b01001110;
	power &=~(1<<1); 
	start();
	byte_write(address);
	byte_write(power);
	stop();
	return power;
}
unsigned char power_off_kran(unsigned char power)
{
	unsigned char address;
	address = 0b01001110;
	power |=(1<<1); 
	start();
	byte_write(address);
	byte_write(power);
	stop();
	return power;
}
unsigned char open_kran(unsigned char power)
{
	unsigned char address;
	address = 0b01001110;
	power &=~(1<<0);
	start();
	byte_write(address);
	byte_write(power);
	stop();
	return power;
}
unsigned char close_kran(unsigned char power)
{
	unsigned char address;
	address = 0b01001110;
	power |=(1<<0);
	start();
	byte_write(address);
	byte_write(power);
	stop();
	return power;
}
//Управление нагревателем
unsigned char power_on_heater(unsigned char power)
{
	unsigned char address;
	address = 0b01001110;
	power |=(1<<4);
	start();
	byte_write(address);
	byte_write(power);
	stop();
	return power;
}
unsigned char power_off_heater(unsigned char power)
{
	unsigned char address;
	address = 0b01001110;
	power &=~(1<<4);
	start();
	byte_write(address);
	byte_write(power);
	stop();
	return power;
}