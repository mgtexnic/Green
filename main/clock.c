

#include "header/clock.h"
#include "header/var1.h"
#include "header/lcd.h"
#include "header/i2c.h"


unsigned char dec_to_bcd(unsigned char data)
{
	unsigned char a, b, data_bcd;
	
	a = data/10;
	b = data- a*10;
	data_bcd = 0xff;
	data_bcd &= a<<4;
	data_bcd |= b;
	
	return data_bcd;
}
unsigned char read_sec(unsigned char reg)
{
	unsigned char out_data;
	out_data = Receive_DS1307(reg);
	return out_data;
}
unsigned char read_min(unsigned char reg)
{
	unsigned char out_data;
	out_data = Receive_DS1307(reg);
	return out_data;
}
unsigned char read_hours(unsigned char reg)
{
	unsigned char out_data;
	out_data = Receive_DS1307(reg);
	return out_data;
}
unsigned char read_day(unsigned char reg)
{
	unsigned char out_data;
	out_data = Receive_DS1307(reg);
	return out_data;
}
unsigned char read_chislo(unsigned char reg)
{
	unsigned char out_data;
	out_data = Receive_DS1307(reg);
	return out_data;
}
unsigned char read_month(unsigned char reg)
{
	unsigned char out_data;
	out_data = Receive_DS1307(reg);
	return out_data;
}
unsigned char read_year(unsigned char reg)
{
	unsigned char out_data;
	out_data = Receive_DS1307(reg);
	return out_data;
}
void write_sec (unsigned char data, unsigned char reg)
{
	Transmit_DS1307(data, reg);
}
void write_min (unsigned char data, unsigned char reg)
{
	Transmit_DS1307(data, reg);
}
void write_hours (unsigned char data, unsigned char reg)
{
	Transmit_DS1307(data, reg);
}
void write_day (unsigned char data, unsigned char reg)
{
	Transmit_DS1307(data, reg);
}
void write_chislo (unsigned char data, unsigned char reg)
{
	Transmit_DS1307(data, reg);
}
void write_month (unsigned char data, unsigned char reg)
{
	Transmit_DS1307(data, reg);
}
void write_year (unsigned char data, unsigned char reg)
{
	Transmit_DS1307(data, reg);
}
void read_time()
{
	time_1307.sec = read_sec(0x00);
	time_1307.min = read_min(0x01);
	time_1307.hours = read_hours(0x02);
	time_1307.day = read_day(0x03);
	time_1307.chislo = read_chislo(0x04);
	time_1307.month = read_month(0x05);
	time_1307.year = read_year(0x06);
}
void rewrite_time()
{
	time_1307.min_RW = time_1307.min;
	time_1307.hours_RW = time_1307.hours;
	time_1307.day_RW = time_1307.day;
	time_1307.chislo_RW = time_1307.chislo;
	time_1307.month_RW = time_1307.month;
	time_1307.year_RW = time_1307.year;
}
void time_bcd_to_hex()
{
	time_1307.min = bcd_to_hex(time_1307.min);
	time_1307.hours = bcd_to_hex(time_1307.hours);
	time_1307.day = bcd_to_hex(time_1307.day);
	time_1307.chislo = bcd_to_hex(time_1307.chislo);
	time_1307.month = bcd_to_hex(time_1307.month);
	time_1307.year = bcd_to_hex(time_1307.year);
}
void write_time(unsigned char sec, unsigned char min, unsigned char hours, unsigned char day,
unsigned char chislo, unsigned char month, unsigned char year)
{
	write_sec(0x00, sec);
	write_min(0x01, min);
	write_hours(0x02, hours);
	write_day(0x03, day);
	write_chislo(0x04, chislo);
	write_month(0x05, month);
	write_year(0x06, year);
}
void set_sec_1307(unsigned char sec, unsigned char address) //Вывод секунд
{
	ascii_lcd_i2c(sec, 0x01, address);						
}
void set_min_1307(unsigned char min, unsigned char address) //Вывод минут
{
	ascii_lcd_i2c(min, 0x01, address);						
}
void set_hours_1307(unsigned char hours, unsigned char address) //Вывод часов
{
	ascii_lcd_i2c(hours, 0x01, address);						 
}
void set_day_1307(unsigned char day, unsigned char address) //Вывод дня недели
{
	switch(day)									
	{
		case 1:
		lcd_putstring_i2c((char*)"Mo", address); //понедельник
		break;
		case 2:
		lcd_putstring_i2c((char*)"Tu", address); //вторник
		break;
		case 3:
		lcd_putstring_i2c((char*)"We", address); //среда
		break;
		case 4:
		lcd_putstring_i2c((char*)"Th", address); //четверг
		break;
		case 5:
		lcd_putstring_i2c((char*)"Fr", address); //пятница
		break;
		case 6:
		lcd_putstring_i2c((char*)"Sa", address); //суббота
		break;
		case 7:
		lcd_putstring_i2c((char*)"Su", address); //воскресенье
		break;
	}
}
void set_chislo_1307(unsigned char data, unsigned char address) //Вывод дня месяца
{
	ascii_lcd_i2c(data, 0x01, address);						
}
void set_month_1307(unsigned char month, unsigned char address) //Вывод месяца
{
	switch(month)								
	{
		case 1:
		lcd_putstring_i2c((char*)"Jan", address); //январь
		break;
		case 2:
		lcd_putstring_i2c((char*)"Feb", address); //февраль
		break;
		case 3:
		lcd_putstring_i2c((char*)"Mar", address); //март
		break;
		case 4:
		lcd_putstring_i2c((char*)"Apr", address); //апрель
		break;
		case 5:
		lcd_putstring_i2c((char*)"May", address); //май
		break;
		case 6:
		lcd_putstring_i2c((char*)"Jun", address); //июнь
		break;
		case 7:
		lcd_putstring_i2c((char*)"Jul", address); //июль
		break;
		case 8:
		lcd_putstring_i2c((char*)"Aug", address); //август
		break;
		case 9:
		lcd_putstring_i2c((char*)"Sep", address); //сентябрь
		break;
		case 16:
		lcd_putstring_i2c((char*)"Oct", address); //октябрь
		break;
		case 17:
		lcd_putstring_i2c((char*)"Nov", address); //ноябрь
		break;
		case 18:
		lcd_putstring_i2c((char*)"Dec", address); //декабрь
		break;
	}
}
void set_year_1307(unsigned char year, unsigned char address) //Вывод года
{
	lcd_putstring_i2c((char*)"20", address);
	ascii_lcd_i2c(year, 0x01, address);					
}
void clock_read (unsigned char sec, unsigned char min, unsigned char hours, unsigned char day,
unsigned char data, unsigned char month, unsigned char year, unsigned char address)
{
	lcd_gotoxy_i2c(0x49, address);
	set_min_1307(min, address);		//Вывод минут
	lcd_gotoxy_i2c(0x46, address);
	set_hours_1307(hours, address);	//Вывод часов
	lcd_gotoxy_i2c(0x54, address);
	set_day_1307(day, address);		//Вывод дня недели
	lcd_gotoxy_i2c(0x59, address);
	set_chislo_1307(data, address);	//Вывод дня месяца
	lcd_gotoxy_i2c(0x5C, address);
	set_month_1307(month, address);	//Вывод месяца
	lcd_gotoxy_i2c(0x60, address);
	set_year_1307(year, address);	//Вывод года
}
void set_clock(unsigned char hours, unsigned char min, unsigned char sec)
{
	unsigned char hours_bcd, min_bcd, sec_bcd;
	
	hours_bcd = dec_to_bcd(hours);
	min_bcd = dec_to_bcd(min);
	sec_bcd = dec_to_bcd(sec);
	
	write_hours(hours_bcd, 0x02);
	write_min(min_bcd, 0x01);
	write_sec(sec_bcd, 0x00);	
}
void set_calendar(unsigned char year, unsigned char month, unsigned char date, unsigned char day,
unsigned char hours, unsigned char min, unsigned char sec)
{
	unsigned char a, b, year_bcd, month_bcd, date_bcd, day_bcd, hours_bcd, min_bcd, sec_bcd;
	 
	year_bcd = dec_to_bcd(year);
	
	a = month/10;
	b = month- a*10;
	month_bcd = 0xff;
	if (month <= 9)
	{
		month_bcd &= b;
	}
	else
	{
		month_bcd &= b;
		month_bcd |= (1<<4);
	}
	date_bcd = dec_to_bcd(date);
	day_bcd = dec_to_bcd(day);
	hours_bcd = dec_to_bcd(hours);
	min_bcd = dec_to_bcd(min);
	sec_bcd = dec_to_bcd(sec);

	write_year(year_bcd, 0x06);
	write_month(month_bcd, 0x05);
	write_chislo(date_bcd, 0x04);
	write_day(day_bcd, 0x03);
	write_hours(hours_bcd, 0x02);
	write_min(min_bcd, 0x01);
	write_sec(sec_bcd, 0x00);
}
void on_clock()
{
	Transmit_DS1307(0x00, 0x00);
}
void clock_power_out(unsigned char data)
{
	Transmit_DS1307(data, 0x07);
}
