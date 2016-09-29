

#include <stdio.h>
#include <avr/io.h>
#define F_CPU 20000000
#include <util/delay.h>

#include "header/io.h"
#include "header/lcd.h"
#include "header/1_wire.h"
#include "header/var1.h"

/*
* read_rom_com - 33h
* skip_rom_com - CCh
* match_rom_com - 55h
* convert_t_com - 44h
* write_com - BEh
* search_rom_com - F0h
* alarm_search_com - ECh
* write_searchpad_com - 4Eh
* copy_searchpad_com - 48h
* recall_e2_com - B8h
* read_power_supply_com - B4h
*/


/*программа формирования на шине 1-wire тайм-слот (0)*/
void ow_bit0()
{
	DDRB |=(1<<1);			//установить бит 1 в регистре DDRB (1)
	PORTB |=(1<<1);			//установить бит 1 в регистре PORTB (1)
	_delay_us(1);
	PORTB &=~(1<<1);		//сбросить бит 1 в регистре PORTB (0)
	_delay_us(70);
	PORTB |=(1<<1);			//установить бит 1 в регистре PORTB (1)
	_delay_us(5);
}
/*программа формирования на шине 1-wire тайм-слот (1)*/
void ow_bit1()
{
	DDRB |=(1<<1);			//установить бит 1 в регистре DDRB (1)
	PORTB |=(1<<1);			//установить бит 1 в регистре PORTB (1)
	_delay_us(1);
	PORTB &=~(1<<1);		//сбросить бит 1 в регистре PORTB (0)
	_delay_us(5);
	PORTB |=(1<<1);			//установить бит 1 в регистре PORTB (1)
	_delay_us(70);
}
void reset_1_wire()
{
	DDRB |=(1<<1);			//установка (1) бита №1 в регистре направления DDRB
	PORTB |=(1<<1);			//установка (1) бита №1 в регистре данных PORTB
	_delay_us(1);
	PORTB &=~(1<<1);		//сброс (0) бита №1 в регистре данных PORTB
}
void presence_1_wire()
{
	PORTB |=(1<<1);			//установка (1) бита №1 в регистре данных PORTB
	_delay_us(1);
}
void read_presence()
{	
	DDRB &=~(1<<1);			 //сброс (0) бита №1 в регистре направления DDRB
	_delay_us(1);
	ow_line.presence1 = PINB; //чтение состояния выводов PINB
	_delay_us(80);
	ow_line.presence2 = PINB; //чтение состояния выводов PINB
	DDRB |=(1<<1);
}

/*программа инициализации передачи/приема по шине 1-wire*/
void init_1_wire()
{
	unsigned char presence1 = 0x00;
	unsigned char presence2 = 0x00;
	
	do
	{
		presence1=0x00;
		presence2=0x00;
		DDRB |=(1<<1);			//DDRB 1
		PORTB |=(1<<1);			//PORTB 1
		_delay_us(1);
		PORTB &=~(1<<1);		//PORTB 0
		_delay_us(600);
		PORTB |=(1<<1);			//PORTB 1
		_delay_us(1);
		DDRB &=~(1<<1);			//DDRB 0
		_delay_us(10);
		presence1 = PINB;
		_delay_us(100);
		presence2 = PINB;
		_delay_us(500);
	}
	while (presence1 <= presence2);
}
/*программа записи выбранной команды по шине 1-wire */
void ow_write_com(unsigned char com)
{
	for(char n=0x00; n<0x08; n++)
	{
		if((_BV(n) & com) == _BV(n)) //выполнить если бит n в переменной установлен 
		{
			ow_bit1();
		}
		else
		{
			ow_bit0();
		}
	}
}
/*программа установки одного бита в полубайте (чтение)*/
unsigned char ow_read_bit_nibble(unsigned char bit_nibble, unsigned char data, unsigned char r)
{
	if(data&(1<<1))
	{
		bit_nibble |=(1<<r);
	}
	else
	{
		bit_nibble &=~(1<<r);
	}
	
	return bit_nibble; 
}
/*программа чтения полубайта по шине 1-wire*/
unsigned char ow_read_nibble()
{
	unsigned char r;
	unsigned char nibble = 0x00;
	unsigned char data = 0x00;
	
	for (r = 0; r < 4; r++)
	{
		DDRB |=(1<<1);			//установить бит 1 в регистре DDRB (1)
		PORTB |=(1<<1);			//установить бит 1 в регистре PORTB (1)
		_delay_us(1);
		PORTB &=~(1<<1);		//сбросить бит 1 в регистре PORTB (0)
		_delay_us(5);
		PORTB |=(1<<1);			//установить бит 1 в регистре PORTB (1)
		_delay_us(1);
		DDRB &=~(1<<1);			//сбросить бит 1 в регистре DDRB (0)
		_delay_us(1);
		data = PINB;
		nibble =  ow_read_bit_nibble(nibble, data, r);
		_delay_us(65);
	}
	
	return nibble;
}
/*программа чтения байта по шине 1-wire*/
unsigned char ow_read_byte()
{
	char byte, nibble_H, nibble_L;
	
	nibble_L = ow_read_nibble ();		// H-nibble
	byte = nibble_L;
	nibble_H = ow_read_nibble ();		// L-nibble
	nibble_H =  nibble_H<<4;
	byte |= nibble_H;
	return byte;
}
void ow_select_temp_index(char ind, unsigned char address)
{
	switch (ind)
	{
		case 0x00:
		lcd_putstring_i2c((char*)",0", address); //,0
		break;
		case 0x01:
		lcd_putstring_i2c((char*)",0625", address); //,0625
		break;
		case 0x02:
		lcd_putstring_i2c((char*)",125", address); //,125
		break;
		case 0x03:
		lcd_putstring_i2c((char*)",1875", address); //,1875
		break;
		case 0x04:
		lcd_putstring_i2c((char*)",25", address); //,25
		break;
		case 0x05:
		lcd_putstring_i2c((char*)",3125", address); //,3125
		break;
		case 0x06:
		lcd_putstring_i2c((char*)",375", address); //,375
		break;
		case 0x07:
		lcd_putstring_i2c((char*)",4375", address); //,4375
		break;
		case 0x08:
		lcd_putstring_i2c((char*)",5", address); //,5
		break;
		case 0x09:
		lcd_putstring_i2c((char*)",5625", address); //,5625
		break;
		case 0x0A:
		lcd_putstring_i2c((char*)",625", address); //,625
		break;
		case 0x0B:
		lcd_putstring_i2c((char*)",6875", address); //,6875
		break;
		case 0x0C:
		lcd_putstring_i2c((char*)",75", address); //,75
		break;
		case 0x0D:
		lcd_putstring_i2c((char*)",8125", address); //,8125
		break;
		case 0x0E:
		lcd_putstring_i2c((char*)",875", address); //,875
		break;
		case 0x0F:
		lcd_putstring_i2c((char*)",9375", address); //,9375
	}
}
/*программа записи 64-битного кода (номера) датчика DS18S20*/
void ow_write_64_code(unsigned char temp[])
{
	signed char n;
	unsigned char simbol;
	
	for (n = 0x07; n>= 0x00; n--)
	{
		simbol = temp[n];
		ow_write_com(simbol);
	}
}
/*программа чтения пакета данных (8 байт) по шине 1-wire*/
void ow_read_data(unsigned char *data7, unsigned char *data6,unsigned char *data5,
unsigned char *data4, unsigned char *data3, unsigned char *data2,
unsigned char *data1, unsigned char *data0)
{
	signed char n;
	char byte;
	
	for (n = 0x01; n<= 0x08; n++)
	{
		byte = ow_read_byte();	// read byte
		
		switch (n)
		{
			case 0x01:
			*data7 = byte;
			break;
			case 0x02:
			*data6 = byte;
			break;
			case 0x03:
			*data5 = byte;
			break;
			case 0x04:
			*data4 = byte;
			break;
			case 0x05:
			*data3 = byte;
			break;
			case 0x06:
			*data2 = byte;
			break;
			case 0x07:
			*data1 = byte;
			break;
			case 0x08:
			*data0 = byte;
			break;
		}
	}
}
/*программа чтения температуры с датчика DS18S20 (один датчик на шине)*/
void ow_read_temp(unsigned char *tmp8, unsigned char *tmp7, unsigned char *tmp6,
unsigned char *tmp5, unsigned char *tmp4, unsigned char *tmp3,
unsigned char *tmp2, unsigned char *tmp1, unsigned char *tmp0)
{
	signed char n;
	char byte;
	
	init_1_wire(); //initialization
	ow_write_com(0xCC); //skip_rom_com
	ow_write_com(0x44); //convert_t_com.03
	_delay_ms(800);
	init_1_wire(); //initialization
	ow_write_com(0xCC); //skip_rom_com
	ow_write_com(0xBE); //write_com
	
	for (n = 0x00; n<= 0x08; n++)
	{
		byte = ow_read_byte();	// read byte
		
		switch (n)
		{
			case 0x00:
			*tmp0 = byte;
			break;
			case 0x01:
			*tmp1 = byte;
			break;
			case 0x02:
			*tmp2 = byte;
			break;
			case 0x03:
			*tmp3 = byte;
			break;
			case 0x04:
			*tmp4 = byte;
			break;
			case 0x05:
			*tmp5 = byte;
			break;
			case 0x06:
			*tmp6 = byte;
			break;
			case 0x07:
			*tmp7 = byte;
			break;
			case 0x08:
			*tmp8 = byte;
			break;
		}
	}
}
void ow_read_select_temp_2_byte(unsigned char *tmp1, unsigned char *tmp0,
unsigned char temp[])
{
	signed char n;
	char byte;
	
	init_1_wire(); //initialization
	ow_write_com(0x55); //match_rom_com
	ow_write_64_code(temp); //write number
	ow_write_com(0x44); //convert_t_com
	_delay_ms(800);
	init_1_wire(); //initialization
	ow_write_com(0x55); //match_rom_com
	ow_write_64_code(temp); //write number
	ow_write_com(0xBE); //write_com
	
	for (n = 0x00; n<= 0x01; n++)
	{
		byte = ow_read_byte();	// read byte
		
		switch (n)
		{
			case 0x00:
			*tmp0 = byte;
			break;
			case 0x01:
			*tmp1 = byte;
			break;
		}
	}
	
	init_1_wire(); //initialization
}
/*программа чтения данных с датчика DS18S20 (более одного датчика на шине, выбор по номеру)*/
void ow_read_select_temp(unsigned char *tmp8, unsigned char *tmp7, unsigned char *tmp6,
unsigned char *tmp5, unsigned char *tmp4, unsigned char *tmp3,
unsigned char *tmp2, unsigned char *tmp1, unsigned char *tmp0,
unsigned char temp[])
{
	signed char n;
	char byte;
	
	init_1_wire(); //initialization
	ow_write_com(0x55); //match_rom_com
	ow_write_64_code(temp); //write number
	ow_write_com(0x44); //convert_t_com
	_delay_ms(800);
	init_1_wire(); //initialization
	ow_write_com(0x55); //match_rom_com
	ow_write_64_code(temp); //write number
	ow_write_com(0xBE); //write_com
	
	for (n = 0x00; n<= 0x08; n++)
	{
		byte = ow_read_byte();	// read byte
		
		switch (n)
		{
			case 0x00:
			*tmp0 = byte;
			break;
			case 0x01:
			*tmp1 = byte;
			break;
			case 0x02:
			*tmp2 = byte;
			break;
			case 0x03:
			*tmp3 = byte;
			break;
			case 0x04:
			*tmp4 = byte;
			break;
			case 0x05:
			*tmp5 = byte;
			break;
			case 0x06:
			*tmp6 = byte;
			break;
			case 0x07:
			*tmp7 = byte;
			break;
			case 0x08:
			*tmp8 = byte;
			break;
		}
	}
}
void t_write_2_byte(unsigned char temp[], unsigned char n)
{
	ow_read_select_temp_2_byte(&ow_line.tmp1, &ow_line.tmp0, temp);
	ow_line.t[n] = ow_line.tmp0>>4;
	ow_line.t[n] |= ow_line.tmp1<<4;	//значение измеренной температуры
	ow_line.i[n] = ow_line.tmp0&0x0f;
	if (ow_line.tmp1&(1<<7))	//проверка на отрицательную температуру
	{
		ow_line.t[n] = ~ow_line.t[n];
		ow_line.i[n] = ~ow_line.i[n];
		ow_line.i[n] = ow_line.i[n]&0x0f;
	}
	//ow_line.t[n] = hex_to_dec(ow_line.t[n]); //перевод значения температуры в десятичный формат
	ow_line.z[n] = ow_line.tmp1;
}
/*программа чтения температуры с датчика DS18S20 и преобразование результата в десятичный вид*/
void t_write(unsigned char temp[], unsigned char n)
{
	ow_read_select_temp(&ow_line.tmp8, &ow_line.tmp7, &ow_line.tmp6, &ow_line.tmp5, &ow_line.tmp4, &ow_line.tmp3, &ow_line.tmp2, &ow_line.tmp1, &ow_line.tmp0, temp);
	ow_line.t[n] = ow_line.tmp0>>4;
	ow_line.t[n] |= ow_line.tmp1<<4;	//значение измеренной температуры
	ow_line.i[n] = ow_line.tmp0&0x0f;
	if (ow_line.tmp1&(1<<7))	//проверка на отрицательную температуру
	{
		ow_line.t[n] = ~ow_line.t[n];
		ow_line.i[n] = ~ow_line.i[n];
		ow_line.i[n] = ow_line.i[n]&0x0f;
	}
	ow_line.t[n] = hex_to_dec(ow_line.t[n]); //перевод значения температуры в десятичный формат
	ow_line.z[n] = ow_line.tmp1;
}
/*программа чтения номера датчика DS18S20(только один датчик на шине)*/
void ow_read_number()
{
	unsigned char address;
	address = 0b01001100;
	
	init_1_wire();
	ow_write_com(0x33);
	ow_read_data(&ow_line.data7, &ow_line.data6, &ow_line.data5, &ow_line.data4, &ow_line.data3, &ow_line.data2, &ow_line.data1, &ow_line.data0);
	ascii_lcd_i2c(ow_line.data0, 0x01, address);
	ascii_lcd_i2c(ow_line.data1, 0x01, address);
	ascii_lcd_i2c(ow_line.data2, 0x01, address);
	ascii_lcd_i2c(ow_line.data3, 0x01, address);
	ascii_lcd_i2c(ow_line.data4, 0x01, address);
	ascii_lcd_i2c(ow_line.data5, 0x01, address);
	ascii_lcd_i2c(ow_line.data6, 0x01, address);
	ascii_lcd_i2c(ow_line.data7, 0x01, address);
}
/*программа определения знака температуры*/
void ow_znak(unsigned char data, unsigned char address)
{
	if (data&(1<<7))
	{
		 lcd_putstring_i2c((char*)"-", address);
	} 
	else
	{
		 lcd_putstring_i2c((char*)"+", address);
	}
}
void ow_control_temp()
{
	unsigned char n;
	
	for (n=0; n<3; n++)
	{
		switch (n)
		{
			case 0: t_write_2_byte(ow_line.temp1, n);
			break;
			case 1: t_write_2_byte(ow_line.temp2, n);
			break;
			case 2: t_write_2_byte(ow_line.temp3, n);
			break;
			case 3: t_write_2_byte(ow_line.temp4, n);
			break;
			case 4: t_write_2_byte(ow_line.temp5, n);
			break;
			case 5: t_write_2_byte(ow_line.temp6, n);
			break;
			case 6: t_write_2_byte(ow_line.temp7, n);
			break;
			case 7: t_write_2_byte(ow_line.temp8, n);
			break;
			case 8: t_write_2_byte(ow_line.temp9, n);
			break;
			case 9: t_write_2_byte(ow_line.temp10, n);
			break;
			case 10: t_write_2_byte(ow_line.temp11, n);
			break;
		}
	}
}

void ow_write_eeprom(unsigned char H, unsigned char L, unsigned char config, unsigned char temp[])
{
	unsigned char n;
	
	init_1_wire(); //initialization
	ow_write_com(0x55); //match_rom_com
	ow_write_64_code(temp); //write number
	ow_write_com(0x4E); //write_searchpad_com
	
	for (n = 1; n<= 3; n++)
	{
		switch (n)
		{
			case 0x00: ow_write_com(H);
			break;
			case 0x01: ow_write_com(L);
			break;
			case 0x02: ow_write_com(config);
			break;
		}
	}
	
}
