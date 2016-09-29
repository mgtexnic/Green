/*
 * main.cpp
 *
 * Created: 10.04.2015 15:27:59
 * Author : Mixail Goncharenko
 *
 *
 * Программа управления термоконтроллером
 *
 */
 
#include <stdio.h>
#include <avr/io.h>
#define F_CPU 20000000		//Частота кварцевого резонатора
#include <avr/interrupt.h>
#include <util/delay.h>

#include "header/var1.h"
#include "header/var.h"
#include "header/io.h"
#include "header/lcd.h"
#include "header/butt.h"
#include "header/1_wire.h"
#include "header/i2c.h"
#include "header/clock.h"
#include "header/drive.h"
#include "header/main.h"


void off_wdt() //выключение сторожевого таймера
{
	cli();
	asm ("wdr");
	MCUSR &=~(1<<WDRF);
	WDTCSR |=(1<<WDCE)|(1<<WDE);
	WDTCSR = 0x00;
	sei();
}
void reset_wdt() //сброс сторожевого таймера
{
	cli();
	asm ("wdr");
	sei();
}
void start_adc() //Запуск преобразования АЦП
{
	ADCSRA |=(1<<ADSC);
	_delay_us(200);
	ADCSRA &=~(1<<ADSC);
	setup.high_byte = ADCL;
	setup.low_byte = ADCH;
}
void init_Data(void) //инициализация переменых
{
	setup.cWatReq = 0;
	setup.cTimeWater = 0;
	setup.cTapPause = 0;
	setup.cLedReq = REQ_LED;
	setup.cDots = REQ_DOTS;
	setup.cWdt = REQ_WDT;
	setup.cSwReq = REQ_SWICH;
	setup.cTimeReq = REQ_TIME;
	setup.cPowReq = REQ_POWER;
	setup.cSenTReq = REQ_SENS_TEMP;
	setup.cTimeUpdate = REQ_TIME_UPDATE;
	setup.ButRewIn |= (1<<1);
	setup.ButRewOut |= (1<<1);
	setup.butshift = 1;
	
	setup.SenseHeater = 0;
	setup.SenseWater = 0;
	setup.DoorSenseOpen = 0;
	setup.DoorSenseClose = 0;
	setup.hours_water_start = 20; //23 max
	setup.min_water_start = 50; //59 max
	setup.hours_water_power = 0; //
	setup.min_water_power = 3; //
	setup.t_in_min = 26;
	setup.t_in_max = 29;
	setup.t_ground_min = 26;
	setup.t_ground_max = 29;
	setup.power_day = 1;
	
	setup.light_level_day = 0x00;
	setup.light_level_night = 0x00;
	
	setup.open_close |= (1<<0); 
	setup.set_num = 0xff;
	setup.number = 0x01;
	setup.power = 0xe7;
	setup.a = 0x00;
	setup.var = 0;
	setup.button = 0;
	setup.shift = 0x01;
	setup.run0 = 0x00;
	setup.run1 = 0x00;
	setup.run2 = 0x00;
	setup.run3 = 0x00;
	setup.data_l1 = 0x00;
	setup.data_l2 = 0x00;
	setup.data_h1 = 0x00;
	setup.data_h2 = 0x00;
	
	time_1307.sec = 0; //секунды
	time_1307.min = 49; //минуты
	time_1307.hours = 20; //часы
	time_1307.day = 2; // день недели
	time_1307.chislo = 9; // день месяца
	time_1307.month = 2; //месяц
	time_1307.year = 16; //год
	time_1307.min_RW = 0;
	time_1307.hours_RW = 0;
	time_1307.day_RW = 0;
	time_1307.chislo_RW = 0;
	time_1307.month_RW = 0;
	time_1307.year_RW = 0;
}
void power_control(unsigned char address)
{
//	проверка температуры земли
//*******************************************
	
	if (setup.t_ground_min > ow_line.t[0]) //температура земли меньше минимальной
	{
		if (!(setup.power & (1<<4))) //включить подогрев если выключен 
		{
			setup.power = power_on_heater(setup.power);
			lcd_gotoxy_i2c(0x5b, address);
			lcd_putstring_i2c((char*)"on", address);
			setup.SenseHeater = 1; 
		}
	}
	if (setup.t_ground_max < ow_line.t[0]) //температура земли больше максимальной
	{
		if (setup.power & (1<<4) ) // выключить подогрев если включен 
		{
			setup.power = power_off_heater(setup.power);
			lcd_gotoxy_i2c(0x5b, address);
			lcd_putstring_i2c((char*)"off", address);
			setup.SenseHeater = 0; 
		}
	}
//	проверка внутренней температуры	
//*******************************************	
	
	if ( setup.t_in_min > ow_line.t[1] ) //внутренняя температура меньше минимальной
	{
		if (!(setup.power & (1<<2))) //дверь открыта (бит №2 сброшен)
		{
			setup.power = close_door(setup.power); //установить бит №2
			setup.power = power_on_door(setup.power); //установить бит №3
			setup.DoorSenseClose = 1; //ожидать сброса бита PD3
		}
	}
	if (setup.t_in_max >= ow_line.t[2]) //открывать если внутренняя температура больше или равна максимальной допустимой внешней
	{
		if (setup.t_in_max < ow_line.t[1]) //внутренняя температура больше максимальной
		{
			if (setup.power & (1<<2)) //дверь закрыта (бит №2 установлен)
			{
				setup.power = open_door(setup.power); //сбросить бит №2
				setup.power = power_on_door(setup.power); //установить бит №3
				setup.DoorSenseOpen = 1; //ожидать сброса бита PD2
			}
		}
	}
}
void water_control()
{
	if (ow_line.t[0] >= setup.t_ground_min) //открывать кран только если температура земли находится в заданном диапазоне
	{
		if (ow_line.t[0] <= setup.t_ground_max)
		{
			if ( !setup.SenseWater )
			{
				//открытие крана
				//****************************************	
				ReadRtc1307();
				setup.cTimeWater = setup.Tpower;
				setup.power = open_kran(setup.power);
				setup.power = power_on_kran(setup.power);
				setup.cTapPause = 0;
				setup.cTap = REQ_TAP;
			} 
		}
	}
	if ( setup.SenseWater )
	{
		//закрытие крана
		//****************************************	
		ReadRtc1307();
		setup.cWatReq = setup.Tstart;
		setup.power = close_kran(setup.power);
		setup.power = power_on_kran(setup.power);
		setup.cTapPause = 0;
		setup.cTap = REQ_TAP;
	}
}

//*******************************************************************//
//								setup
//*******************************************************************//

void system_setup()
{
	cli(); 
	init_port(); //инициализация портов
	init_tWi();	//инициализация I2C 
	init_adc(); //инициализация АЦП
	lcd_init_i2c(); //инициализация ЖКИ 
    lcd_clear_i2c(0x4c); //очистка ЖКИ
	init_interrupt(); //инициализация внешних прерываний (кнопки, концевики)
	off_wdt(); //остановка сторожеваго таймера
	init_wdt(); //инициализация сторожевого таймера
	PORTD = 0x3f;
	sei();	
}

//*******************************************************************//

void write_data(unsigned char a_lcd)
{
	while(1)
	{
		lcd_clear_i2c(a_lcd);
		set_time(a_lcd); //установка времени
		do
		{
			if ( !(setup.ButRewOut & (1<<1)) )
			{
				return;
			} 
			swiches(setup.set_num, a_lcd);
		}
		while (setup.number == 1);
		lcd_clear_i2c(a_lcd);
		set_date(a_lcd);	//установка даты
		do
		{
			if ( !(setup.ButRewOut & (1<<1)) )
			{
				return;
			}
			swiches(setup.set_num, a_lcd);
		}
		while (setup.number == 2);
		lcd_clear_i2c(a_lcd);
		set_t_in(a_lcd); //установка контроллируемых значений внутренней температуры
		do
		{
			if ( !(setup.ButRewOut & (1<<1)) )
			{
				return;
			}
			swiches(setup.set_num, a_lcd);
		}
		while (setup.number == 3);
		lcd_clear_i2c(a_lcd);
		set_t_ground(a_lcd);	//установка контроллируемых значений температуры поверхности
		do
		{
			if ( !(setup.ButRewOut & (1<<1)) )
			{
				return;
			}
			swiches(setup.set_num, a_lcd);
		}
		while (setup.number == 4);
		lcd_clear_i2c(a_lcd);
		set_water_start(a_lcd);	//установка контроллируемых значений начала полива
		do
		{
			if ( !(setup.ButRewOut & (1<<1)) )
			{
				return;
			}
			swiches(setup.set_num, a_lcd);
		}
		while (setup.number == 5);
		lcd_clear_i2c(a_lcd);
		set_water_power(a_lcd);	//установка контроллируемых значений продолжительности полива
		do
		{
			if ( !(setup.ButRewOut & (1<<1)) )
			{
				return;
			}
			swiches(setup.set_num, a_lcd);
		}
		while (setup.number == 6);
		lcd_clear_i2c(a_lcd);
		set_power_day(a_lcd);	//установка значения переодичности полива
		do
		{
			if ( !(setup.ButRewOut & (1<<1)) )
			{
				return;
			}
			swiches(setup.set_num, a_lcd);
		}
		while (setup.number == 7);
	}
}

void stop_interrupt0()
{
	PCICR &=~(1<<PCIE0);
}

void start_interrupt0()
{
	PCICR |=(1<<PCIE0);
}

//*******************************************************************//

int main()
{	
	unsigned char a_lcd, a_lcd1;
	
	a_lcd = 0b01001100;
	a_lcd1 = 0b01001110;
	
	/*Начальные установки*/	
	system_setup(); //инициализация встроенных и внешних модулей
	init_Data(); //установка начальных значений переменных
	TWI_Transmit(a_lcd1, setup.power);
	write_data(a_lcd); //установка начальнах рабочих значений 
	lcd_clear_i2c(a_lcd);
	lcd_putbyte_i2c(LCD_COMMAND, 0b00001100, a_lcd); //выключение курсора и мигания
	on_clock(); //включение модуля DS1307(часы-календарь)
	clock_power_out(0x11); //настройка модуля DS1307(выходной сигнал на выводе SQW с частотой 4096 kГц)
	set_calendar(time_1307.year, time_1307.month, time_1307.chislo, time_1307.day, time_1307.hours, time_1307.min, time_1307.sec); //запись начальных значений в модуль RTC
	ow_control_temp(); //первичное считывание температуры с датчиков
	ReadRtc1307(); //считывание времени с модуля RTC
	setup.cWatReq = setup.Tstart;
	setup.n = setup.power_day;
	setup.button = 1;
	data_out_lcd(a_lcd);
	rewrite_time();
	set_data_clock(a_lcd);
	
	while(1)
	{
		if ( !setup.cLedReq ) //таймер мигания светодиода (индикация состояния работы)
		{
			setup.cLedReq = REQ_LED;
			PORTB ^=(1 << PB2);
		}
		if ( !setup.cDots ) //таймер двоеточия часов
		{
			setup.cDots = REQ_DOTS;
			lcd_gotoxy_i2c(0x02, a_lcd); //2-строка
			setup.var_dots ^= (1<<0);
			if (setup.var_dots & (1<<0))
			{
				lcd_putstring_i2c((char*)" ", a_lcd);
			} 
			else
			{
				lcd_putstring_i2c((char*)":", a_lcd);
			}
		}
		if ( !setup.cWdt ) //таймер сброса сторожевого таймера
		{
			setup.cWdt = REQ_WDT;
			reset_wdt();
		}
		if ( !setup.cSwReq ) //таймер опроса кнопок и концевиков
		{
			setup.cSwReq = REQ_SWICH;
			swiches(setup.set_num, a_lcd); 
		}
		if ( !setup.cTimeReq ) //
		{
			setup.cTimeReq = REQ_TIME;
			read_time();
		}
		if ( !setup.cTimeUpdate ) //таймер обновления времени
		{
			setup.cTimeUpdate = REQ_TIME_UPDATE;
			if ( time_1307.min_RW != time_1307.min )
			{
				update_min(a_lcd);
				
				if ( time_1307.hours_RW != time_1307.hours )
				{
					update_hours(a_lcd);
					
					if ( time_1307.day_RW != time_1307.day )
					{
						update_day(a_lcd);
						
						if ( time_1307.chislo_RW != time_1307.chislo )
						{
							update_chislo(a_lcd);
							
							if ( time_1307.month_RW != time_1307.month )
							{
								update_month(a_lcd);
								
								if ( time_1307.year_RW != time_1307.year )
								{
									update_year(a_lcd);
									time_1307.year_RW = time_1307.year;
								}
								
								time_1307.month_RW = time_1307.month;
							}
							
							time_1307.chislo_RW = time_1307.chislo;
						}
						
						time_1307.day_RW = time_1307.day;
					}
					
					time_1307.hours_RW = time_1307.hours;
				}
				
				time_1307.min_RW = time_1307.min;
			}
		}
		if ( !setup.cPowReq ) //таймер проверки соответствия температуры внутри и земли заданным пределам  
		{
			setup.cPowReq = REQ_POWER;
			power_control(a_lcd);
		}
		if ( !setup.cWatReq ) //таймер начала полива 
		{
			if(setup.n > 0)
			{
				setup.cWatReq = DAY;
			}
			else
			{
				if ( !setup.cTimeWater ) //таймер продолжительности полива
				{
					water_control();
				}
			}
		}
		if ( !setup.cTapPause ) 
		{
			if ( !setup.cTap ) //таймер ожидания закрытия/открытия крана
			{
				setup.power = power_off_kran(setup.power);
				if (!(setup.power&(1<<0)))
				{
					setup.SenseWater = 1; //кран открыт
					lcd_gotoxy_i2c(0x65, a_lcd); 
					lcd_putstring_i2c((char*)"on",a_lcd);
				}
				else
				{
					setup.SenseWater = 0; //кран закрыт
					lcd_gotoxy_i2c(0x65, a_lcd); 
					lcd_putstring_i2c((char*)"off", a_lcd);
				}
				setup.cTapPause = 1;
			}
		}
		if ( !setup.cSenTReq ) //таймер опроса датчиков температуры
		{
			setup.cSenTReq = REQ_SENS_TEMP;
			ow_control_temp();
			write_temp_lcd(a_lcd);
		}
		if ( !(setup.ButRewIn & (1<<1)) ) //таймер опроса кнопки входа в режим корректировки установленных значений
		{
			stop_interrupt0(); //остановка счетчиков
			setup.ButRewOut |= (1<<1);
			setup.button = 0; //разблокировка кнопок управления
			time_bcd_to_hex();
			write_data(a_lcd); //корректировка начальнах рабочих значений
			set_calendar(time_1307.year, time_1307.month, time_1307.chislo, time_1307.day, time_1307.hours, time_1307.min, time_1307.sec); //запись начальных значений в модуль RTC
			lcd_clear_i2c(a_lcd);
			data_out_lcd(a_lcd); //вывод текущих значений на экран
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
			set_data_clock(a_lcd); //вывод текушего времени и даты
			setup.ButRewIn |= (1<<1);
			rewrite_time();
			lcd_putbyte_i2c(LCD_COMMAND, 0b00001100, a_lcd); //выключение курсора и мигания
			start_interrupt0(); //запуск счетчиков 
			setup.button = 1; //блокировка кнопок управления кроме кнопки входа в режим корректировки
		}
	}
}

void ReadRtc1307()
{
	uint8_t sec, min, hours;
	
	read_time(); // чтение времени с DS1307
	sec = bcd_to_hex(time_1307.sec);
	min = bcd_to_hex(time_1307.min);
	hours = bcd_to_hex(time_1307.hours);
	
	//расчет текущеего времени
	time_1307.time = ((hours*3600UL + min*60UL + sec)*4096UL)*2; 
	
	setup.Time = time_1307.time;
	
	//расчет времени начала открытия крана
	setup.StTime = ((setup.hours_water_start*3600 + setup.min_water_start*60UL)*4096)*2;
	
	//расчет времени открытого состояния крана 
	setup.Tpower = ((setup.hours_water_power*3600UL + setup.min_water_power*60UL)*4096UL)*2;
	
	//расчет времени срабатывания открытия крана при разных начальных и текущих значениях
	if (setup.Time > setup.StTime)
	{
		setup.Tstart = ((((TIME-hours)+setup.hours_water_start)*3600UL + setup.min_water_start*60UL)*4096UL)*2;
	}
	else
	{
		setup.Tstart = (((setup.hours_water_start-hours)*3600UL + setup.min_water_power*60UL)*4096UL)*2;
	}
	setup.nTime = time_1307.time;
}

ISR(ADC_vect)	//Обработчик прерывания (АЦП)
{
	setup.high_byte = ADCL;
	setup.low_byte = ADCH;
}
ISR(WDT_vect)	//Обработчик прерывания (сторожевой таймер)
{
	asm ("wdr");
}
ISR(PCINT0_vect)	//Обработчик внешнего прерывания (RTC)
{
	setup.nTime++;
	if ( setup.nTime >= 707788800 )setup.nTime = 0;
	
	if ( setup.cLedReq )setup.cLedReq--;
	if ( setup.cSwReq )setup.cSwReq--;
	if ( setup.cTimeReq )setup.cTimeReq--;
	if ( setup.cDots )setup.cDots--; 
	if ( setup.cWdt )setup.cWdt--;
	if ( setup.cTimeUpdate )setup.cTimeUpdate--;
	if ( setup.cPowReq )setup.cPowReq--;
	if ( setup.cWatReq )setup.cWatReq--;
	if ( setup.cTimeWater )setup.cTimeWater--;
	if ( setup.cTap )setup.cTap--;
	if ( setup.cSenTReq )setup.cSenTReq--;
}
ISR(PCINT2_vect)	//Обработчик внешнего прерывания (кнопки, концевики)
{
	setup.butt_var = PIND;
	read_swich(setup.butt_var);
}