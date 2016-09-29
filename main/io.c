
#include <stdio.h>
#include <avr/io.h>
#define F_CPU 20000000		//������� ���������� ����������
#define F_TWI 80000			//������� ���� TWI	
#include <avr/interrupt.h>
#include "header/io.h"


void init_port()
{
	DDRB |=(1<<PB5)|(1<<PB4)|(1<<PB3)|(1<<PB2)|(1<<PB1);
	PORTB=0x00;
	DDRC &=~(1<<PC5)|(1<<PC4)|(1<<PC3)|(1<<PC2)|(1<<PC1)|(1<<PC0);
	PORTC=0x00;
	DDRD &=~(1<<PD7)|(1<<PD6)|(1<<PD5)|(1<<PD4)|(1<<PD3)|(1<<PD2);
	DDRD |=(1<<PD1)|(1<<PD0);
	PORTD=0x00;
}
void init_adc()
{
	//��� ���������
	ADCSRA |=0x00;
	//����� ������������ �������������� (������ ��� ADATE=1)
	ADCSRB |=(0<<ADTS2)|(0<<ADTS1)|(0<<ADTS0);
	//���������� ���, ������������ ���������� ������, ������������ ������� PC0
	ADMUX |=(0<<REFS1)|(1<<REFS0)|(0<<ADLAR)|(0<<MUX3)|(0<<MUX2)|(0<<MUX1)|(0<<MUX0);
	//��������������� �������, ����� ���������� ��������������, ����������� ������� 128 (��� 20 ���)
	ADCSRA |=(1<<ADEN)|(0<<ADSC)|(1<<ADATE)|(1<<ADPS2)|(1<<ADPS1)|(1<<ADPS0);
}
void init_tWi()
{
	TWSR &=(~((1<<TWPS1)|(1<<TWPS0)));
	TWBR = ((F_CPU/F_TWI)-16)/2;
}
void init_timer16()
{
	//TSM - ��������� ���� �������������, PSRSYNC - ����� ���� �������������
	GTCCR = (0<<TSM)|(0<<PSRSYNC);
	//���������� �� ���������� A � B
	TIMSK1 = (1<<OCIE1B)|(1<<OCIE1A);
	//������ �������� �� ������� �����������, ����� ��� ����������
	TCCR1A = (0<<COM1A1)|(0<<COM1A0)|(0<<COM1B1)|(0<<COM1B0)|(0<<WGM11)|(0<<WGM10);
	//������ ���������� (������ ��� N = 256)
	TCCR1B = (0<<WGM13)|(1<<WGM12)|(0<<CS12)|(0<<CS11)|(0<<CS10);
	//������� �������
	TCNT1H = 0x00;
	TCNT1L = 0x00;
	//������� ��������� A (1 ��)
	OCR1AH = 0x4C;
	OCR1AL = 0x96;
	//������� ��������� B (1 ��)
	OCR1BH = 0x4C;
	OCR1BL = 0x96;
}
void init_wdt()
{
	//������ ����������� �������, ������ �� ����������, ����-��� 4 ���
	cli();
	WDTCSR |=(1<<WDCE)|(1<<WDE);
	WDTCSR = (1<<WDIE)|(0<<WDE)|(1<<WDP3)|(0<<WDP2)|(0<<WDP1)|(0<<WDP0);
	sei();
}
void init_interrupt()
{
	cli();
	PCICR = (1<<PCIE2)|(1<<PCIE0);
	PCMSK0 = (1<<PCINT0);
	PCMSK2 = (1<<PCINT23)|(1<<PCINT22)|(1<<PCINT21)|(1<<PCINT20)|(1<<PCINT19)|(1<<PCINT18);
	sei();
}

