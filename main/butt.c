

#include <stdio.h>
#include <avr/io.h>
#define F_CPU 20000000
#include <util/delay.h>
#include <avr/interrupt.h>

#include "header/io.h"
#include "header/var1.h"
#include "header/lcd.h"
#include "header/butt.h"
#include "header/clock.h"
#include "header/drive.h"



void  read_swich(unsigned char num1)
{
	if (!(num1 & (1<<4))) //выполнить если бит №4 сброшен (нажата кнопка №1)
	{
		setup.set_num &=~(1<<0); //сбрость бит №0 в переменной
	}
	if (!(num1 & (1<<5))) //выполнить если бит №5 сброшен (нажата кнопка №2)
	{
		setup.set_num &=~(1<<1); //сбрость бит №1 в переменной
	}
	if (!(num1 & (1<<6))) //выполнить если бит №6 сброшен (нажата кнопка №3)
	{
		setup.set_num &=~(1<<2); //сбрость бит №2 в переменной
	}
	if (!(num1 & (1<<7))) //выполнить если бит №7 сброшен (нажата кнопка №4)
	{
		setup.set_num &=~(1<<3); //сбрость бит №3 в переменной
	}
	if (!(num1 & (1<<2))) //выполнить если бит №2 сброшен (замкнут концевик №2)
	{
		if (setup.DoorSenseOpen)
		{
			setup.set_num &=~(1<<4); //сбрость бит №4 в переменной
			setup.DoorSenseOpen = 0;
		}
	}
	if (!(num1 & (1<<3))) //выполнить если бит №3 сброшен (замкнут концевик №3)
	{
		if (setup.DoorSenseClose)
		{
			setup.set_num &=~(1<<5); //сбрость бит №5 в переменной
			setup.DoorSenseClose = 0;
		}
	}
}

void selec_num(unsigned char set, unsigned char address)
{
	switch(set)
	{
		case 0x00:	lcd_putchar_i2c('0', address);
		break;
		case 0x01:	lcd_putchar_i2c('1', address);
		break;
		case 0x02:	lcd_putchar_i2c('2', address);
		break;
		case 0x03:	lcd_putchar_i2c('3', address);
		break;
		case 0x04:	lcd_putchar_i2c('4', address);
		break;
		case 0x05:	lcd_putchar_i2c('5', address);
		break;
		case 0x06:	lcd_putchar_i2c('6', address);
		break;
		case 0x07:	lcd_putchar_i2c('7', address);
		break;
		case 0x08:	lcd_putchar_i2c('8', address);
		break;
		case 0x09:	lcd_putchar_i2c('9', address);
		break;
	}
}
void button_1(unsigned char num)
{
	if (!(num & (1<<0))) //выполнить если бит №0 сброшен //переключение между командами настройки
	{
		for (unsigned char x=0; x<6; x++)
		{
			unsigned char pin_read0 = PIND;
			_delay_ms(10);
			if (!(pin_read0 & (1<<4))) //выполнить если бит №0 сброшен
			{
				setup.run0++;
				
				if (setup.run0 > 0x05)	//фиксация события "нажатие кнопки"
				{
					unsigned char up_pin0;
					
					do
					{
						up_pin0 = PIND;			//фиксация события "отжатие кнопки"
					}
					while (!(up_pin0 & (1<<4))); //выполнять пока бит №0 сброшен
					
					if (setup.number > 0x06)
					{
						setup.number = 0x00;
					}
					//lcd_clear();
					switch (setup.number)
					{
						case 0x00:	//set_time(); //выполнение команды set_time()
						setup.shift=0x01;
						break;
						case 0x01:	//set_date(); //выполнение команды set_date()
						setup.shift=0x01;
						break;
						case 0x02:	//set_t_in(); //выполнение команды set_t_in()
						setup.shift=0x01;
						break;
						case 0x03:	//set_t_ground();	//выполнение команды set_t_ground()
						setup.shift=0x01;
						break;
						case 0x04:	//set_water_start(); //выполнение команды set_water_start()
						setup.shift=0x01;
						break;
						case 0x05:	//set_water_power(); //выполнение команды set_water_power()
						setup.shift=0x01;
						break;
						case 0x06:	//set_power_day(); //выполнение команды set_power_day()
						setup.shift=0x01;
						break;
					}
					
					setup.number++;
					setup.run0=0x00;
				}
			}
			else
			{
				setup.run0=0x00;
			}
			
			setup.set_num = 0xff;
			num = 0xff;
		}
	}
}
void button_2(unsigned char num, unsigned char address)
{
	if (!(num & (1<<1))) //выполнить если бит №1 сброшен //выбор значения для корректировки
	{
		for (unsigned char x=0; x<6; x++)
		{
			unsigned char pin_read1 = PIND;
			_delay_ms(10);
			if (!(pin_read1 & (1<<5))) //выполнить если бит №1 сброшен
			{
				setup.run1++;
				
				if (setup.run1 > 0x05)	//фиксация события "нажатие кнопки"
				{
					unsigned char up_pin1;
					
					do
					{
						up_pin1 = PIND;			//фиксация события "отжатие кнопки"
					}
					while (!(up_pin1 & (1<<5))); //выполнять пока бит №1 сброшен
					
					setup.number--;
					if (!(setup.number == 0x01))
					{
						if (setup.shift>=0x04)
						{
							setup.shift=0x01;
						}
						else
						{
							setup.shift++;
						}
					}
					else
					{
						if (setup.shift>=0x07)
						{
							setup.shift=0x01;
						}
						else
						{
							setup.shift++;
						}
					}
					switch (setup.number)
					{
						case 0x00:	
						switch (setup.shift)
						{
							case 0x01:	 lcd_gotoxy_i2c(0x1C, address);	//set_time();
							break;
							case 0x02:	lcd_gotoxy_i2c(0x1D, address);	//set_time();
							break;
							case 0x03:	lcd_gotoxy_i2c(0x24, address);	//set_time();
							break;
							case 0x04:	lcd_gotoxy_i2c(0x25, address);	//set_time();
							break;
						}
						break;
						case 0x01:
						switch (setup.shift)
						{
							case 0x01:	lcd_gotoxy_i2c(0x1A, address);	//set_date();
							break;
							case 0x02:	lcd_gotoxy_i2c(0x24, address);	//set_date();
							break;
							case 0x03:	lcd_gotoxy_i2c(0x25, address);	//set_date();
							break;
							case 0x04:	lcd_gotoxy_i2c(0x5B, address);	//set_date();
							break;
							case 0x05:	lcd_gotoxy_i2c(0x66, address);	//set_date();
							break;
							case 0x06:	lcd_gotoxy_i2c(0x67, address);	//set_date();
							break;
						}
						break;
						case 0x02:	
						switch(setup.shift)
						{
							case 0x01:	lcd_gotoxy_i2c(0x1A, address);	//set_t_in();
							break;
							case 0x02:	lcd_gotoxy_i2c(0x1B, address);	//set_t_in();
							break;
							case 0x03:	lcd_gotoxy_i2c(0x24, address);	//set_t_in();
							break;
							case 0x04:	lcd_gotoxy_i2c(0x25, address);	//set_t_in();
							break;
						}
						break;
						case 0x03:	
						switch(setup.shift)
						{
							case 0x01:	lcd_gotoxy_i2c(0x1A, address);	//set_t_ground();
							break;
							case 0x02:	lcd_gotoxy_i2c(0x1B, address);	//set_t_ground();
							break;
							case 0x03:	lcd_gotoxy_i2c(0x24, address);	//set_t_ground();
							break;
							case 0x04:	lcd_gotoxy_i2c(0x25, address);	//set_t_ground();
							break;
						}
						break;
						case 0x04:	
						switch(setup.shift)
						{
							case 0x01:	lcd_gotoxy_i2c(0x1C, address);	//set_water_start();
							break;
							case 0x02:	lcd_gotoxy_i2c(0x1D, address);	//set_water_start();
							break;
							case 0x03:	lcd_gotoxy_i2c(0x24, address);	//set_water_start();
							break;
							case 0x04:	lcd_gotoxy_i2c(0x25, address);	//set_water_start();
							break;
						}
						break;
						case 0x05:	
						switch(setup.shift)
						{
							case 0x01:	lcd_gotoxy_i2c(0x1C, address);	//set_water_power();
							break;
							case 0x02:	lcd_gotoxy_i2c(0x1D, address);	//set_water_power();
							break;
							case 0x03:	lcd_gotoxy_i2c(0x24, address);	//set_water_power();
							break;
							case 0x04:	lcd_gotoxy_i2c(0x25, address);	//set_water_power();
							break;
						}
						break;
						case 0x06:
						lcd_gotoxy_i2c(0x23, address);	//set_power_day();
						break;
					}
					setup.run1=0x00;
					setup.number++;
				}
			}
			else
			{
				setup.run1=0x00;
			}
			
			setup.set_num = 0xff;
			num = 0xff;
		}
	}
}
void button_3(unsigned char num, unsigned char address)
{
	if (!(num & (1<<2))) //выполнить если бит №3 сброшен // корректировка выбранного значения - "-"
	{
		for (unsigned char x=0; x<6; x++)
		{
			unsigned char pin_read2 = PIND;
			_delay_ms(10);
			if (!(pin_read2 & (1<<6))) //выполнить если бит №3 сброшен
			{
				setup.run2++;
				
				if (setup.run2 > 0x05)
				{
					unsigned char up_pin2;
					
					do
					{
						up_pin2 = PIND;		//фиксация события "отжатие кнопки"
					}
					while (!(up_pin2 & (1<<6))); //выполнять пока бит №1 сброшен
					
					setup.number--;
					
					switch (setup.number)
					{
						
						case 0x00:
						
						setup.data_h1 = time_1307.hours/10;
						setup.data_l1 = time_1307.hours - 10*setup.data_h1;
						setup.data_h2 = time_1307.min/10;
						setup.data_l2 = time_1307.min - 10*setup.data_h2;
						
						switch(setup.shift)	// режим определен, выбор  позиции курсора
						{
							case 0x01:	//(#1) позиция определена, инкремент переменной - set_time()						
							if (setup.data_h1 >= 0x02)
							{
								setup.data_h1 = 0x00;
							}
							else
							{
								setup.data_h1++;
							}
							lcd_gotoxy_i2c(0x1C, address);
							selec_num(setup.data_h1, address);
							lcd_gotoxy_i2c(0x1C, address);
							time_1307.hours = (10*setup.data_h1)+setup.data_l1;
							break;
							case 0x02:	//(#2) позиция определена, инкремент переменной - set_time()
							if (setup.data_h1 < 0x02)
							{
								if (setup.data_l1 >= 0x09)
								{
									setup.data_l1 = 0x00;
								}
								else
								{
									setup.data_l1++;
								}
							} 
							else
							{
								if (setup.data_l1 >= 0x03)
								{
									setup.data_l1 = 0x00;
								}
								else
								{
									setup.data_l1++;
								}
							}
							lcd_gotoxy_i2c(0x1D, address);
							selec_num(setup.data_l1, address);
							lcd_gotoxy_i2c(0x1D, address);
							time_1307.hours = (10*setup.data_h1)+setup.data_l1;
							break;
							case 0x03:	//(#3) позиция определена, инкремент переменной - set_time()
							if (setup.data_h2 >= 0x05)
							{
								setup.data_h2 = 0x00;
							}
							else
							{
								setup.data_h2++;
							}
							lcd_gotoxy_i2c(0x24, address);
							selec_num(setup.data_h2, address);
							lcd_gotoxy_i2c(0x24, address);
							time_1307.min = (10*setup.data_h2)+setup.data_l2;
							break;
							case 0x04:	//(#4) позиция определена, инкремент переменной - set_time()
							if (setup.data_l2 >= 0x09)
							{
								setup.data_l2 = 0x00;
							}
							else
							{
								setup.data_l2++;
							}
							lcd_gotoxy_i2c(0x25, address);
							selec_num(setup.data_l2, address);
							lcd_gotoxy_i2c(0x25, address);
							time_1307.min = (10*setup.data_h2)+setup.data_l2;
							break;
						}
						break;
						case 0x01: 
						
						setup.data_h1 = time_1307.chislo/10;
						setup.data_l1 = time_1307.chislo - 10*setup.data_h1;
						setup.data_h2 = time_1307.year/10;
						setup.data_l2 = time_1307.year - 10*setup.data_h2;
						
						switch(setup.shift) // режим определен, выбор  позиции курсора
						{
							case 0x01: //(#1) позиция определена, инкремент переменной - set_date()
							if (time_1307.day >= 0x07)
							{
								time_1307.day = 1;
							}
							else
							{
								time_1307.day++;
							}
							lcd_gotoxy_i2c(0x19, address);
							set_day_1307(time_1307.day, address);
							lcd_gotoxy_i2c(0x19, address);
							break;
							case 0x02: //(#2) позиция определена, инкремент переменной - set_date()
							if (setup.data_h1 >= 0x03)
							{
								setup.data_h1 = 0x00;
							}
							else
							{
								setup.data_h1++;
							}
							lcd_gotoxy_i2c(0x24, address);
							selec_num(setup.data_h1, address);
							lcd_gotoxy_i2c(0x24, address);
							time_1307.chislo = (10*setup.data_h1)+setup.data_l1;
							break;
							case 0x03: //(#3) позиция определена, инкремент переменной - set_date()
							if (setup.data_h1 <= 0x02)
							{
								if (setup.data_l1 >= 0x09)
								{
									setup.data_l1 = 0x00;
								}
								else
								{
									setup.data_l1++;
								}
							}
							else
							{
								if (setup.data_l1 >= 0x01)
								{
									setup.data_l1 = 0x00;
								}
								else
								{
									setup.data_l1++;
								}
							}
							lcd_gotoxy_i2c(0x25, address);
							selec_num(setup.data_l1, address);
							lcd_gotoxy_i2c(0x25, address);
							time_1307.chislo = (10*setup.data_h1)+setup.data_l1;
							break;
							case 0x04: //(#4) позиция определена, инкремент переменной - set_date()
							if (time_1307.a >= 12)
							{
								time_1307.a = 1;
								time_1307.month = dec_to_bcd(time_1307.a);
							}
							else
							{
								time_1307.a++;
								time_1307.month = dec_to_bcd(time_1307.a);
							}
							lcd_gotoxy_i2c(0x5B, address);
							set_month_1307(time_1307.month, address);
							lcd_gotoxy_i2c(0x5B, address);
							break;
							case 0x05: //(#5) позиция определена, инкремент переменной - set_date()
							if (setup.data_h2 >= 0x09)
							{
								setup.data_h2 = 0x00;
							}
							else
							{
								setup.data_h2++;
							}
							lcd_gotoxy_i2c(0x66, address);
							selec_num(setup.data_h2, address);
							lcd_gotoxy_i2c(0x66, address);
							time_1307.year = (10*setup.data_h2)+setup.data_l2;
							break;
							case 0x06: //(#6) позиция определена, инкремент переменной - set_date()
							if (setup.data_l2 >= 0x09)
							{
								setup.data_l2 = 0x00;
							}
							else
							{
								setup.data_l2++;
							}
							lcd_gotoxy_i2c(0x67, address);
							selec_num(setup.data_l2, address);
							lcd_gotoxy_i2c(0x67, address);
							time_1307.year = (10*setup.data_h2)+setup.data_l2;
							break;
						}
						break;
						case 0x02:
						
						setup.data_h1 = setup.t_in_min/10;
						setup.data_l1 = setup.t_in_min - 10*setup.data_h1;
						setup.data_h2 = setup.t_in_max/10;
						setup.data_l2 = setup.t_in_max - 10*setup.data_h2;
						
						switch(setup.shift)	// режим определен, выбор  позиции курсора
						{
							case 0x01:	//(#1) позиция определена, инкремент переменной - set_t_in()
							if (setup.data_h1 >= 0x02)
							{
								setup.data_h1 = 0x00;
							}
							else
							{
								setup.data_h1++;
							}
							lcd_gotoxy_i2c(0x1A, address);
							selec_num(setup.data_h1, address);
							lcd_gotoxy_i2c(0x1A, address);
							setup.t_in_min = (10*setup.data_h1)+setup.data_l1;
							break;
							case 0x02:	//(#2) позиция определена, инкремент переменной - set_t_in()
							if (setup.data_l1 >= 0x09)
							{
								setup.data_l1 = 0x00;
							}
							else
							{
								setup.data_l1++;
							}
							lcd_gotoxy_i2c(0x1B, address);
							selec_num(setup.data_l1, address);
							lcd_gotoxy_i2c(0x1B, address);
							setup.t_in_min = (10*setup.data_h1)+setup.data_l1;
							break;
							case 0x03:	//(#3) позиция определена, инкремент переменной - set_t_in()
							if (setup.data_h2 >= 0x02)
							{
								setup.data_h2 = 0x00;
							}
							else
							{
								setup.data_h2++;
							}
							lcd_gotoxy_i2c(0x24, address);
							selec_num(setup.data_h2, address);
							lcd_gotoxy_i2c(0x24, address);
							setup.t_in_max = (10*setup.data_h2)+setup.data_l2;
							break;
							case 0x04:	//(#4) позиция определена, инкремент переменной - set_t_in()
							if (setup.data_l2 >= 0x09)
							{
								setup.data_l2 = 0x00;
							}
							else
							{
								setup.data_l2++;
							}
							lcd_gotoxy_i2c(0x25, address);
							selec_num(setup.data_l2, address);
							lcd_gotoxy_i2c(0x25, address);
							setup.t_in_max = (10*setup.data_h2)+setup.data_l2;
							break;
						}
						break;
						case 0x03:
						
						setup.data_h1 = setup.t_ground_min/10;
						setup.data_l1 = setup.t_ground_min - 10*setup.data_h1;
						setup.data_h2 = setup.t_ground_max/10;
						setup.data_l2 = setup.t_ground_max - 10*setup.data_h2;
						
						switch(setup.shift)	// режим определен, выбор  позиции курсора
						{
							case 0x01:	//(#1) позиция определена, инкремент переменной - set_t_ground()
							if (setup.data_h1 >= 0x02)
							{
								setup.data_h1 = 0x00;
							}
							else
							{
								setup.data_h1++;
							}
							lcd_gotoxy_i2c(0x1A, address);
							selec_num(setup.data_h1, address);
							lcd_gotoxy_i2c(0x1A, address);
							setup.t_ground_min = (10*setup.data_h1)+ setup.data_l1;
							break;
							case 0x02:	//(#2) позиция определена, инкремент переменной - set_t_ground()
							if (setup.data_l1 >= 0x09)
							{
								setup.data_l1 = 0x00;
							}
							else
							{
								setup.data_l1++;
							}
							lcd_gotoxy_i2c(0x1B, address);
							selec_num(setup.data_l1, address);
							lcd_gotoxy_i2c(0x1B, address);
							setup.t_ground_min = (10*setup.data_h1)+ setup.data_l1;
							break;
							case 0x03:	//(#3) позиция определена, инкремент переменной - set_t_ground()
							if (setup.data_h2 >= 0x02)
							{
								setup.data_h2 = 0x00;
							}
							else
							{
								setup.data_h2++;
							}
							lcd_gotoxy_i2c(0x24, address);
							selec_num(setup.data_h2, address);
							lcd_gotoxy_i2c(0x24, address);
							setup.t_ground_max = (10*setup.data_h2)+setup.data_l2;
							break;
							case 0x04:	//(#4) позиция определена, инкремент переменной - set_t_ground()
							if (setup.data_l2 >= 0x09)
							{
								setup.data_l2 = 0x00;
							}
							else
							{
								setup.data_l2++;
							}
							lcd_gotoxy_i2c(0x25, address);
							selec_num(setup.data_l2, address);
							lcd_gotoxy_i2c(0x25, address);
							setup.t_ground_max = (10*setup.data_h2)+setup.data_l2;
							break;
						}
						break;
						case 0x04:	
						
						setup.data_h1 = setup.hours_water_start/10;
						setup.data_l1 = setup.hours_water_start - 10*setup.data_h1;
						setup.data_h2 = setup.min_water_start/10;
						setup.data_l2 = setup.min_water_start - 10*setup.data_h2;
						
						switch(setup.shift)	// режим определен, выбор  позиции курсора
						{
							case 0x01:	//(#1) позиция определена, инкремент переменной - set_water_start()
							if (setup.data_h1 >= 0x02)
							{
								setup.data_h1 = 0x00;
							}
							else
							{
								setup.data_h1++;
							}
							lcd_gotoxy_i2c(0x1C, address);
							selec_num(setup.data_h1, address);
							lcd_gotoxy_i2c(0x1C, address);
							setup.hours_water_start = (10*setup.data_h1)+setup.data_l1;
							break;
							case 0x02:	//(#2) позиция определена, инкремент переменной - set_water_start()
							if (setup.data_h1 < 0x02)
							{
								if (setup.data_l1 >= 0x09)
								{
									setup.data_l1 = 0x00;
								}
								else
								{
									setup.data_l1++;
								}
							}
							else
							{
								if (setup.data_l1 >= 0x03)
								{
									setup.data_l1 = 0x00;
								}
								else
								{
									setup.data_l1++;
								}
							}
							lcd_gotoxy_i2c(0x1D, address);
							selec_num(setup.data_l1, address);
							lcd_gotoxy_i2c(0x1D, address);
							setup.hours_water_start = (10*setup.data_h1)+setup.data_l1;
							break;
							case 0x03:	//(#3) позиция определена, инкремент переменной - set_water_start()
							if (setup.data_h2 >= 0x05)
							{
								setup.data_h2 = 0x00;
							}
							else
							{
								setup.data_h2++;
							}
							lcd_gotoxy_i2c(0x24, address);
							selec_num(setup.data_h2, address);
							lcd_gotoxy_i2c(0x24, address);
							setup.min_water_start = (10*setup.data_h2)+setup.data_l2;
							break;
							case 0x04:	//(#4) позиция определена, инкремент переменной - set_water_start()
							if (setup.data_l2 >= 0x09)
							{
								setup.data_l2 = 0x00;
							}
							else
							{
								setup.data_l2++;
							}
							lcd_gotoxy_i2c(0x25, address);
							selec_num(setup.data_l2, address);
							lcd_gotoxy_i2c(0x25, address);
							setup.min_water_start = (10*setup.data_h2)+setup.data_l2;
							break;
						}
						break;
						case 0x05:	
						
						setup.data_h1 = setup.hours_water_power/10;
						setup.data_l1 = setup.hours_water_power - 10*setup.data_h1;
						setup.data_h2 = setup.min_water_power/10;
						setup.data_l2 = setup.min_water_power - 10*setup.data_h2;
						
						switch(setup.shift)	// режим определен, выбор  позиции курсора
						{
							case 0x01:	//(#1) позиция определена, инкремент переменной - set_water_power()
							if (setup.data_h1 >= 0x02)
							{
								setup.data_h1 = 0x00;
							}
							else
							{
								setup.data_h1++;
							}
							lcd_gotoxy_i2c(0x1C, address);
							selec_num(setup.data_h1, address);
							lcd_gotoxy_i2c(0x1C, address);
							setup.hours_water_power = (10*setup.data_h1)+ setup.data_l1;
							break;
							case 0x02:	//(#2) позиция определена, инкремент переменной - set_water_power()
							if (setup.data_h1 < 0x02)
							{
								if (setup.data_l1 >= 0x09)
								{
									setup.data_l1 = 0x00;
								}
								else
								{
									setup.data_l1++;
								}
							}
							else
							{
								if (setup.data_l1 >= 0x03)
								{
									setup.data_l1 = 0x00;
								}
								else
								{
									setup.data_l1++;
								}
							}
							lcd_gotoxy_i2c(0x1D, address);
							selec_num(setup.data_l1, address);
							lcd_gotoxy_i2c(0x1D, address);
							setup.hours_water_power = (10*setup.data_h1)+ setup.data_l1;
							break;
							case 0x03:	//(#3) позиция определена, инкремент переменной - set_water_power()
							if (setup.data_h2 >= 0x05)
							{
								setup.data_h2 = 0x00;
							}
							else
							{
								setup.data_h2++;
							}
							lcd_gotoxy_i2c(0x24, address);
							selec_num(setup.data_h2, address);
							lcd_gotoxy_i2c(0x24, address);
							setup.min_water_power = (10*setup.data_h2)+setup.data_l2;
							break;
							case 0x04:	//(#4) позиция определена, инкремент переменной - set_water_power()
							if (setup.data_l2 >= 0x09)
							{
								setup.data_l2 = 0x00;
							}
							else
							{
								setup.data_l2++;
							}
							lcd_gotoxy_i2c(0x25, address);
							selec_num(setup.data_l2, address);
							lcd_gotoxy_i2c(0x25, address);
							setup.min_water_power = (10*setup.data_h2)+setup.data_l2;
							break;
						}
						break;
						case 0x06:
						
						setup.data_l1 = setup.power_day;
						
						if (setup.data_l1 >= 0x07)
						{
							setup.data_l1 = 0x00;
						}
						else
						{
							setup.data_l1++;
						}
						lcd_gotoxy_i2c(0x23, address);
						selec_num(setup.data_l1, address);
						lcd_gotoxy_i2c(0x23, address);
						setup.power_day = setup.data_l1;
						break;
					}
					setup.run2=0x00;
					setup.number++;
				}
			}
			else
			{
				setup.run2=0x00;
			}
			
			setup.set_num = 0xff;
			num = 0xff;
		}
	}
}
void button_4(unsigned char num)
{
	if (!(num & (1<<3))) //выполнить если бит №3 сброшен // корректировка выбранного значения - "-"
	{
		for (unsigned char x=0; x<6; x++)
		{
			unsigned char pin_read3 = PIND;
			_delay_ms(10);
			if (!(pin_read3 & (1<<7))) //выполнить если бит №3 сброшен
			{
				setup.run3++;
				
				if (setup.run3 > 0x05)
				{
					unsigned char up_pin3;
					
					do
					{
						up_pin3 = PIND;		//фиксация события "отжатие кнопки"
					}
					while (!(up_pin3 & (1<<7))); //выполнять пока бит №1 сброшен
					
					
					if (setup.butshift > 1)
					{
						setup.butshift = 0;
					} 
					switch(setup.butshift)
					{
						case 0: 
						setup.ButRewIn &=~ (1<<1);
						break;
						case 1: 
						setup.ButRewOut &=~ (1<<1);
						break;
					}
					
					setup.butshift++;
					setup.run3 = 0;
				}
			}
			else
			{
				setup.run3=0x00;
			}
			
			setup.set_num = 0xff;
			num = 0xff;
		}
	}
}

void swich_1(unsigned char num)
{
	if (!(num & (1<<4))) //выполнить если бит №4 сброшен
	{
		setup.power = power_off_door(setup.power); //сбросить бит №3
		setup.set_num = 0xff;
	}
}
void swich_2(unsigned char num)
{
	if (!(num & (1<<5))) //выполнить если бит №5 сброшен
	{
		setup.power = power_off_door(setup.power); //сбросить бит №3
		setup.set_num = 0xff;
	}
}
void OpenCloseDoor(unsigned char a_lcd)
{
	setup.open_close ^= (1<<0);
		
	if ( !(setup.open_close & (1<<0)) )
	{
		stop_interrupt0(); //остановка счетчиков
		setup.power = open_door(setup.power); //открыть дверь
	}
	else
	{
		setup.power = close_door(setup.power); //закрыть дверь
		if (setup.SenseHeater)
		{
			lcd_gotoxy_i2c(0x5b, a_lcd);
			lcd_putstring_i2c((char*)"on",a_lcd);
		}
		else
		{
			lcd_gotoxy_i2c(0x5b, a_lcd);
			lcd_putstring_i2c((char*)"off", a_lcd);
		}
		if (setup.SenseWater)
		{
			lcd_gotoxy_i2c(0x65, a_lcd);
			lcd_putstring_i2c((char*)"on",a_lcd);
		}
		else
		{
			lcd_gotoxy_i2c(0x65, a_lcd);
			lcd_putstring_i2c((char*)"off", a_lcd);
		}
		read_time();
		set_data_clock(a_lcd);
		rewrite_time();
		start_interrupt0(); //запуск счетчиков
	}
}

void swiches(unsigned char num, unsigned char address)
{	
	if (num == 0xFE)
	{
		if ( !setup.button )
		{
			if (setup.power & (1<<2))
			{
				OpenCloseDoor(address);
			}
		}
		else
		{
			button_1(num);
		}
	}
	if ( !setup.button )
	{
		switch(num)
		{
			case 0xFD: button_2(num, address);
			break;
			case 0xFB: button_3(num, address);
			break;
		}
	}
	if (num == 0xF7)
	{
		button_4(num);
	}
	switch(num)
	{		
		case 0xEF: swich_1(num);
		break;
		case 0xDF: swich_2(num);
		break;
	}
}
	                                                                                                                                                                                                                                                                                    

