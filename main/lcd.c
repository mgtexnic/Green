
#include <stdio.h>
#include <avr/io.h>
#define F_CPU 20000000
#include <util/delay.h>
#include "header/var1.h"
#include "header/io.h"
#include "header/lcd.h"
#include "header/clock.h"
#include "header/i2c.h"

/*программа преобразования шестнадцатеричного числа в двоичное*/
unsigned char hex_to_bin(unsigned char data)
{
	unsigned char res = 0x00;
	unsigned char a, b, n, s;
	
	a = (data>>4)&0x0f;
	b = data&0x0f;
	
	for (s=1; s<3; s++)
	{
		if (s>1)
		{
			n=b;
			res = res<<4;
		}
		else
		{
			n=a;
		}
		switch(n)
		{
			case 0x00: res |= 0b0000;
			break;
			case 0x01: res |= 0b0001;
			break;
			case 0x02: res |= 0b0100;
			break;
			case 0x03: res |= 0b0011;
			break;
			case 0x04: res |= 0b0100;
			break;
			case 0x05: res |= 0b1001;
			break;
			case 0x06: res |= 0b0110;
			break;
			case 0x07: res |= 0b0111;
			break;
			case 0x08: res |= 0b1000;
			break;
			case 0x09: res |= 0b1001;
			break;
			case 0x0A: res |= 0b1010;
			break;
			case 0x0B: res |= 0b1011;
			break;
			case 0x0C: res |= 0b1100;
			break;
			case 0x0D: res |= 0b1101;
			break;
			case 0x0E: res |= 0b1110;
			break;
			case 0x0F: res |= 0b1111;
			break;
		}
	}
	
	return res;
}
unsigned char hex_to_dec(unsigned char data)
{
	unsigned char x, y, z;
	
	x = data/10;
	y = data - x*10;
	z = x*16 + y*1;
	
	return z; 
}
uint8_t bcd_to_hex(uint8_t data)
{
	return ((data >> 4) * 10)+(data & 0x0F);
}
void update_min(unsigned char address)
{
	unsigned char min;
	
	if (setup.ButRewIn & (1<<1))
	{
		min = bcd_to_hex(time_1307.min);
	} 
	else
	{
		min = time_1307.min;
	}

	lcd_gotoxy_i2c(0x03, address); //2-строка
	sprintf(time_1307.M, "%02u", min);
	lcd_putstring_i2c(time_1307.M, address);
}
void update_hours(unsigned char address)
{
	unsigned char hours;
	
	if (setup.ButRewIn & (1<<1))
	{
		hours = bcd_to_hex(time_1307.hours);
	} 
	else
	{
		hours = time_1307.hours;
	}
	
	lcd_gotoxy_i2c(0x00, address); //2-строка
	sprintf(time_1307.H, "%02u", hours);
	lcd_putstring_i2c(time_1307.H, address);
}
void update_day(unsigned char address)
{
	lcd_gotoxy_i2c(0x07, address); //2-строка
	set_day_1307(time_1307.day, address);
}
void update_chislo(unsigned char address)
{
	unsigned char chislo;
	
	if (setup.ButRewIn & (1<<1))
	{
		chislo = bcd_to_hex(time_1307.chislo);
	} 
	else
	{
		chislo = time_1307.chislo;
	}
	
	lcd_gotoxy_i2c(0x09, address); //2-строка
	sprintf(time_1307.C, "%02u", chislo);
	lcd_putstring_i2c(time_1307.C, address);
}
void update_month(unsigned char address)
{
	lcd_gotoxy_i2c(0x0B, address); //2-строка
	set_month_1307(time_1307.month, address);
}
void update_year(unsigned char address)
{
	unsigned char year;
	
	if (setup.ButRewIn & (1<<1))
	{
		year = bcd_to_hex(time_1307.year);
	} 
	else
	{
		year = time_1307.year;
	}
	
	lcd_gotoxy_i2c(0x10, address); //2-строка
	sprintf(time_1307.Y, "%02u", year);
	lcd_putstring_i2c(time_1307.Y, address);
}
void set_data_clock(unsigned char address)
{
	update_min(address);
	update_hours(address);
	update_day(address);
	update_chislo(address);
	update_month(address);
	update_year(address);
}
void power_on_lcd(unsigned char address) //включение подсветки индикатора
{
	lcd_gotoxy_i2c(0x46, address); //2-строка
	lcd_putstring_i2c((char*)"POWER ON", address);
}
void setup_lcd(unsigned char address) //просмотр установленных параметров
{
	lcd_gotoxy_i2c(0x07, address); //1-строка
	lcd_putstring_i2c((char*)"SETUP", address);
	lcd_gotoxy_i2c(0x14, address); //3-строка
	lcd_putstring_i2c((char*)"1-SET TIME", address); //установка времени
	lcd_gotoxy_i2c(0x54, address); //4-строка
	lcd_putstring_i2c((char*)"2-SET T_IN", address); //установка пределов внутренней температуры
	_delay_ms(1500);
	lcd_clear_i2c(address);
	lcd_gotoxy_i2c(0x07, address); //1-строка
	lcd_putstring_i2c((char*)"SETUP", address);
	lcd_gotoxy_i2c(0x14, address); //3-строка
	lcd_putstring_i2c((char*)"2-SET T_IN", address); //установка пределов внутренней температуры
	lcd_gotoxy_i2c(0x54, address); //4-строка
	lcd_putstring_i2c((char*)"3-SET T_OUT", address); //установка пределов наружней температуры
	_delay_ms(1500);
	lcd_clear_i2c(address);
	lcd_gotoxy_i2c(0x07, address); //1-строка
	lcd_putstring_i2c((char*)"SETUP", address);
	lcd_gotoxy_i2c(0x14, address); //3-строка
	lcd_putstring_i2c((char*)"3-SET T_OUT", address); //установка пределов наружней температуры
	lcd_gotoxy_i2c(0x54, address); //4-строка
	lcd_putstring_i2c((char*)"4-SET T_GROUND", address); //установка пределов температуры земли
	_delay_ms(1500);
	lcd_clear_i2c(address);
	lcd_gotoxy_i2c(0x07, address); //1-строка
	lcd_putstring_i2c((char*)"SETUP", address);
	lcd_gotoxy_i2c(0x14, address); //3-строка
	lcd_putstring_i2c((char*)"4-SET T_GROUND", address); //установка пределов температуры земли
	lcd_gotoxy_i2c(0x54, address); //4-строка
	lcd_putstring_i2c((char*)"5-SET WATER_START", address); //установка времени начала полива
	_delay_ms(1500);
	lcd_clear_i2c(address);
	lcd_gotoxy_i2c(0x07, address); //1-строка
	lcd_putstring_i2c((char*)"SETUP", address);
	lcd_gotoxy_i2c(0x14, address); //3-строка
	lcd_putstring_i2c((char*)"5-SET WATER_START", address); //установка времени начала полива
	lcd_gotoxy_i2c(0x54, address); //4-строка
	lcd_putstring_i2c((char*)"6-SET WATER_POWER", address); //установка времени продолжительности полива
}
void set_time(unsigned char address) //установка времени
{	
	lcd_gotoxy_i2c(0x06, address); //1-строка
	lcd_putstring_i2c((char*)"SET TIME", address);
	lcd_gotoxy_i2c(0x16, address); //3-строка
	lcd_putstring_i2c((char*)"HOURS:", address); //часы
	sprintf(time_1307.H, "%02u", time_1307.hours);
	lcd_putstring_i2c(time_1307.H, address);
	lcd_gotoxy_i2c(0x20, address); //3-строка
	lcd_putstring_i2c((char*)"MIN:", address); //минуты
	sprintf(time_1307.M, "%02u", time_1307.min);
	lcd_putstring_i2c(time_1307.M, address);
	lcd_gotoxy_i2c(0x1C, address);
	lcd_putbyte_i2c(LCD_COMMAND, 0b00001111, address);
}
void set_date(unsigned char address) //установка даты
{
	lcd_gotoxy_i2c(0x06, address);
	lcd_putstring_i2c((char*)"SET DATE", address);
	lcd_gotoxy_i2c(0x15, address);
	lcd_putstring_i2c((char*)"DAY:", address); //день недели
	set_day_1307(time_1307.day, address);
	lcd_gotoxy_i2c(0x1F, address);
	lcd_putstring_i2c((char*)"DATE:", address); //дата
	sprintf(time_1307.C, "%02u", time_1307.chislo);
	lcd_putstring_i2c(time_1307.C, address);
	lcd_gotoxy_i2c(0x55, address);
	lcd_putstring_i2c((char*)"MONTH:", address); //месяц
	set_month_1307(time_1307.month, address);
	lcd_gotoxy_i2c(0x5F, address);
	lcd_putstring_i2c((char*)"YEAR:20", address); //год
	sprintf(time_1307.Y, "%02u", time_1307.year);
	lcd_putstring_i2c(time_1307.Y, address);
	lcd_gotoxy_i2c(0x19, address);
	lcd_putbyte_i2c(LCD_COMMAND, 0b00001111, address);
}
void set_t_in(unsigned char address) //установка пределов внутренней температуры
{
	lcd_gotoxy_i2c(0x06, address); //1-строка
	lcd_putstring_i2c((char*)"SET T_IN", address);
	lcd_gotoxy_i2c(0x16, address); //3-строка
	lcd_putstring_i2c((char*)"MIN=", address); //минимальная температура
	sprintf(setup.H, "%02u", setup.t_in_min);
	lcd_putstring_i2c(setup.H, address);
	lcd_gotoxy_i2c(0x20, address); //3-строка
	lcd_putstring_i2c((char*)"MAX=", address); //максимальная температура
	sprintf(setup.M, "%02u", setup.t_in_max);
	lcd_putstring_i2c(setup.M, address);
	lcd_gotoxy_i2c(0x1A, address);
	lcd_putbyte_i2c(LCD_COMMAND, 0b00001111, address);
}
void set_t_ground(unsigned char address) //установка пределов температуры земли
{	
	lcd_gotoxy_i2c(0x04, address); //1-строка
	lcd_putstring_i2c((char*)"SET T_GROUND", address);
	lcd_gotoxy_i2c(0x16, address); //3-строка
	lcd_putstring_i2c((char*)"MIN=", address); //минимальная температура
	sprintf(setup.H, "%02u", setup.t_ground_min);
	lcd_putstring_i2c(setup.H, address);
	lcd_gotoxy_i2c(0x20, address); //3-строка
	lcd_putstring_i2c((char*)"MAX=", address); //максимальная температура
	sprintf(setup.M, "%02u", setup.t_ground_max);
	lcd_putstring_i2c(setup.M, address);
	lcd_gotoxy_i2c(0x1A, address);
	lcd_putbyte_i2c(LCD_COMMAND, 0b00001111, address);
}
void set_water_start(unsigned char address) //установка времени начала полива (1 раз в день в установленное время)
{
	lcd_gotoxy_i2c(0x02, address); //1-строка
	lcd_putstring_i2c((char*)"SET WATER_START", address);
	lcd_gotoxy_i2c(0x16, address); //3-строка
	lcd_putstring_i2c((char*)"HOURS=", address); //часы
	sprintf(setup.H, "%02u", setup.hours_water_start);
	lcd_putstring_i2c(setup.H, address);
	lcd_gotoxy_i2c(0x20, address); //3-строка
	lcd_putstring_i2c((char*)"MIN=", address); //минуты
	sprintf(setup.M, "%02u", setup.min_water_start);
	lcd_putstring_i2c(setup.M, address);
	lcd_gotoxy_i2c(0x1C, address);
	lcd_putbyte_i2c(LCD_COMMAND, 0b00001111, address);
}
void set_water_power(unsigned char address) //установка времени продолжительности полива
{	
	lcd_gotoxy_i2c(0x02, address); //1-строка
	lcd_putstring_i2c((char*)"SET WATER_POWER", address);
	lcd_gotoxy_i2c(0x16, address); //3-строка
	lcd_putstring_i2c((char*)"HOURS=", address); //часы
	sprintf(setup.H, "%02u", setup.hours_water_power);
	lcd_putstring_i2c(setup.H, address);
	lcd_gotoxy_i2c(0x20, address); //3-строка
	lcd_putstring_i2c((char*)"MIN=", address); //минуты
	sprintf(setup.M, "%02u", setup.min_water_power);
	lcd_putstring_i2c(setup.M, address);
	lcd_gotoxy_i2c(0x1C, address);
	lcd_putbyte_i2c(LCD_COMMAND, 0b00001111, address);
}
void set_power_day(unsigned char address) //установка интервала полива (1 раз в N дней)
{
	lcd_gotoxy_i2c(0x03, address); //1-строка
	lcd_putstring_i2c((char*)"SET POWER_DAY", address);
	lcd_gotoxy_i2c(0x17, address); //3-строка
	lcd_putstring_i2c((char*)"NUMBER_DAY:", address); //
	sprintf(setup.D, "%02u", setup.power_day);
	lcd_putstring_i2c(setup.D, address);  //3-строка
	lcd_gotoxy_i2c(0x23, address);
	lcd_putbyte_i2c(LCD_COMMAND, 0b00001111, address);
}
void data_lcd(unsigned char address) //просмотр установленных значений
{
	lcd_gotoxy_i2c(0x08, address); //1-строка
	lcd_putstring_i2c((char*)"DATA", address);
	lcd_gotoxy_i2c(0x15, address); //1-строка
	lcd_putstring_i2c((char*)"TIME", address);
	lcd_gotoxy_i2c(0x22, address); //1-строка
	ascii_lcd_i2c(setup.hours, 0x01, address);
	lcd_putchar_i2c(':', address);
	ascii_lcd_i2c(setup.min, 0x01, address);
	lcd_gotoxy_i2c(0x55, address); //1-строка
	lcd_putstring_i2c((char*)"T_IN", address);
	lcd_gotoxy_i2c(0x62, address); //1-строка
	ascii_lcd_i2c(setup.t_in_min, 0x01, address);
	lcd_putchar_i2c(':', address);
	ascii_lcd_i2c(setup.t_in_max, 0x01, address);
	_delay_ms(1500);
	lcd_gotoxy_i2c(0x15, address); //1-строка
	lcd_putstring_i2c((char*)"T_IN", address);
	lcd_gotoxy_i2c(0x22, address); //1-строка
	ascii_lcd_i2c(setup.t_in_min, 0x01, address);
	lcd_putchar_i2c(':', address);
	ascii_lcd_i2c(setup.t_in_max, 0x01, address);
	_delay_ms(1500);
	lcd_gotoxy_i2c(0x55, address); //1-строка
	lcd_putstring_i2c((char*)"T_GROUND", address);
	lcd_gotoxy_i2c(0x62, address); //1-строка
	ascii_lcd_i2c(setup.t_ground_min, 0x01, address);
	lcd_putchar_i2c(':', address);
	ascii_lcd_i2c(setup.t_ground_max, 0x01, address);
	_delay_ms(1500);
	lcd_gotoxy_i2c(0x15, address); //1-строка
	lcd_putstring_i2c((char*)"T_GROUND", address);
	lcd_gotoxy_i2c(0x22, address); //1-строка
	ascii_lcd_i2c(setup.t_ground_min, 0x01, address);
	lcd_putchar_i2c(':', address);
	ascii_lcd_i2c(setup.t_ground_max, 0x01, address);
	lcd_gotoxy_i2c(0x55, address); //1-строка
	lcd_putstring_i2c((char*)"WATER_START", address);
	lcd_gotoxy_i2c(0x62, address); //1-строка
	ascii_lcd_i2c(setup.hours_water_start, 0x01, address);
	lcd_putchar_i2c(':', address);
	ascii_lcd_i2c(setup.min_water_start, 0x01, address);
	_delay_ms(1500);
	lcd_gotoxy_i2c(0x15, address); //1-строка
	lcd_putstring_i2c((char*)"WATER_START", address);
	lcd_gotoxy_i2c(0x22, address); //1-строка
	ascii_lcd_i2c(setup.hours_water_start, 0x01, address);
	lcd_putchar_i2c(':', address);
	ascii_lcd_i2c(setup.min_water_start, 0x01, address);
	lcd_gotoxy_i2c(0x55, address); //1-строка
	lcd_putstring_i2c((char*)"WATER_POWER", address);
	lcd_gotoxy_i2c(0x62, address); //1-строка
	ascii_lcd_i2c(setup.hours_water_power, 0x01, address);
	lcd_putchar_i2c(':', address);
	ascii_lcd_i2c(setup.min_water_power, 0x01, address);
}
void data_out_lcd(unsigned char address) //вывод действительных значений на экран
{
	lcd_gotoxy_i2c(0x06, address); //2-строка
	lcd_putstring_i2c((char*)"(       20  )", address);
	lcd_gotoxy_i2c(0x40, address); //2-строка
	lcd_putstring_i2c((char*)"GND_1-4(  :  :  :  )", address);
	lcd_gotoxy_i2c(0x14, address); //3-строка
	lcd_putstring_i2c((char*)"IN_T(  )  OUT_T(  )", address);
	lcd_gotoxy_i2c(0x54, address); //4-строка
	lcd_putstring_i2c((char*)"HEATER:    WATER:   ", address);
}
void write_temp_lcd(unsigned char address)
{
	lcd_gotoxy_i2c(0x48, address); //2-строка
	lcd_putstring_i2c((char*)"  :  :  :  ", address);
	lcd_gotoxy_i2c(0x48, address); //2-строка
	sprintf(ow_line.g0, "%02u", ow_line.t[0]);
	lcd_putstring_i2c(ow_line.g0, address);
	lcd_putchar_i2c(':', address);
	sprintf(ow_line.g0, "%02u", ow_line.t[0]);
	lcd_putstring_i2c(ow_line.g0, address);
	lcd_putchar_i2c(':', address);
	sprintf(ow_line.g0, "%02u", ow_line.t[0]);
	lcd_putstring_i2c(ow_line.g0, address);
	lcd_putchar_i2c(':', address);
	sprintf(ow_line.g0, "%02u", ow_line.t[0]);
	lcd_putstring_i2c(ow_line.g0, address);
	lcd_gotoxy_i2c(0x19, address); //2-строка
	lcd_putstring_i2c((char*)"  ", address);
	lcd_gotoxy_i2c(0x19, address); //2-строка
	sprintf(ow_line.g1, "%02u", ow_line.t[1]);
	lcd_putstring_i2c(ow_line.g1, address);
	lcd_gotoxy_i2c(0x24, address); //2-строка
	lcd_putstring_i2c((char*)"  ", address);
	lcd_gotoxy_i2c(0x24, address); //2-строка
	sprintf(ow_line.g2, "%02u", ow_line.t[2]);
	lcd_putstring_i2c(ow_line.g2, address);
}
void power_off_lcd(unsigned char address) //выключение питания
{
	lcd_gotoxy_i2c(0x46, address); //выключение
	lcd_putstring_i2c((char*)"POWER OFF", address);
}
void heater_ground(unsigned char address)
{
	if (setup.SenseHeater)
	{
		lcd_putstring_i2c((char*)"on", address);
	}
	else
	{
		lcd_putstring_i2c((char*)"off", address);
	}
}
void water_ground(unsigned char address)
{
	if (setup.SenseWater)
	{
		lcd_putstring_i2c((char*)"on", address);
	}
	else
	{
		lcd_putstring_i2c((char*)"off", address);
	}
}
void lcd_putnibble_i2c(unsigned char data, unsigned char nibble, unsigned char address)
{
	start();
	byte_write(address);
	data &=~(1<<1);
	data |=(1<<2)|(1<<3);
	byte_write(data);
	data = data|nibble;
	byte_write(data);
	data &=~(1<<2);
	byte_write(data);
	stop();
}
void lcd_putbyte_i2c(unsigned char rs, unsigned char byte, unsigned char address)
{
	unsigned char data, high, low;
	data = 0x00;
	high = 0x00;
	low = 0x00;
	
	high = byte&0xf0;
	low = byte<<4;
	low = low&0xf0;
	if(rs == 0)
	{
		data &=~ (1<<0);	
	}
	else
	{
		data |= (1<<0);
	}
	lcd_putnibble_i2c(data, high, address);
	lcd_putnibble_i2c(data, low, address);
}
void lcd_putchar_i2c(unsigned char znak, unsigned char address)
{
	unsigned char data, high, low;
	data = 0x00;
	high = 0x00;
	low = 0x00;
	
	data |= (1<<0);
	high = znak&0xf0;
	low = znak<<4;
	low = low&0xf0;
	
	lcd_putnibble_i2c(data, high, address);
	lcd_putnibble_i2c(data, low, address);
}
void lcd_gotoxy_i2c (unsigned char adr, unsigned char address) //установка курсора на выбранную позицию индикатора
{
	adr|=0x80;
	lcd_putbyte_i2c(0x00, adr, address);
}
void lcd_putstring_i2c(char stroka[], unsigned char address) //вывод строки символов на индикатор
{
	unsigned char i;
	
	for (i=0; stroka[i]!='\0'; i++)
	{
		lcd_putchar_i2c (stroka[i], address);
	}
}
void lcd_init_i2c()
{
	unsigned char address;
	address = 0b01001100;
	
	_delay_ms(15);
	lcd_putnibble_i2c(0x00, 0b00110000, address);	//8-wire interface
	_delay_ms(5);
	lcd_putnibble_i2c(0x00, 0b00110000, address);	//8-wire interface
	_delay_us(100);
	lcd_putnibble_i2c(0x00, 0b00110000, address);	//8-wire interface
	_delay_us(1);
	lcd_putnibble_i2c(0x00, 0b00100000, address);	//4-wire interface
	_delay_us(1);
	lcd_putbyte_i2c(0x00, 0b00101000, address);	//4-wire interface, 2-line, size simbol-5x11
	_delay_us(1);
	lcd_putbyte_i2c(0x00, 0b00001100, address);	//Display - on, cursor - off, blinking - off
	_delay_us(1);
	lcd_putbyte_i2c(0x00, 0b00000110, address);	//Shift cursor
	_delay_us(1);
}
void lcd_clear_i2c(unsigned char address) //очистка индикатора
{
	lcd_putbyte_i2c(0x00, 0x01, address);			//Display clear
	_delay_us(1500);
}
void bin_lcd_i2c(unsigned char bin, unsigned char address)
{
	signed char n;
	
	for (n=7; n>=0; n--)
	{
		if (bin&(1<<n))
		{
			lcd_putchar_i2c('1', address);
		}
		else
		{
			lcd_putchar_i2c('0', address);
		}
	}
}
void dec_lcd_i2c(unsigned char dec, char rs, unsigned char address)
{
	const unsigned char asc[10] = {'0','1','2','3','4','5','6','7','8','9'};

	lcd_putbyte_i2c(0x01, asc[((dec>>4)&0x0F)], address);
	lcd_putbyte_i2c(0x01, asc[(dec&0x0F)], address);
}
void ascii_lcd_i2c(unsigned char hex, char rs, unsigned char address)
{
	const unsigned char asc[16] = {'0','1','2','3','4','5','6','7','8','9','A','B','C','D','E','F'};

	lcd_putbyte_i2c(0x01, asc[((hex>>4)&0x0F)], address);
	lcd_putbyte_i2c(0x01, asc[(hex&0x0F)], address);
}