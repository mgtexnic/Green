
#ifndef _LCD_H_
#define _LCD_H_


#define RS_PORT PORTD
#define RS_PIN 4
#define E_PORT PORTD
#define E_PIN 5
#define RW_PORT PORTD
#define RW_PIN 6

#define e_low()		 E_PORT&=~_BV(E_PIN)
#define e_high()	 E_PORT|=_BV(E_PIN)
#define rs_low()	 RS_PORT&=~_BV(RS_PIN)
#define rs_high()	 RS_PORT|=_BV(RS_PIN)
#define rw_low()	 RW_PORT&=~_BV(RW_PIN)
#define rw_high()	 RW_PORT|=_BV(RW_PIN)

#define LCD_COMMAND 0x00			//������ �������
#define LCD_DATA    0x01			//������ ������

unsigned char hex_to_bin(unsigned char data); //������� ������������������ ����� � ��������
unsigned char hex_to_dec(unsigned char data); //������� ������������������ ����� � ����������
uint8_t bcd_to_hex(uint8_t data);
void update_min(unsigned char address);
void update_hours(unsigned char address);
void update_day(unsigned char address);
void update_chislo(unsigned char address);
void update_month(unsigned char address);
void update_year(unsigned char address);
void set_data_clock(unsigned char address);
void power_on_lcd(unsigned char address); //��������� ��������� ����������
void setup_lcd(unsigned char address); //�������� ������������� ����������
void set_time(unsigned char address); //��������� �������
void set_date(unsigned char address); //��������� ����
void set_t_in(unsigned char address); //��������� �������� ���������� �����������
void set_t_out(void); //��������� �������� �������� �����������
void set_t_ground(unsigned char address); //��������� �������� ����������� �����
void set_water_start(unsigned char address); //��������� ������� ������ ������
void set_water_power(unsigned char address); //��������� ������� ����������������� ������
void set_power_day(unsigned char address); //��������� ��������� ������ (���)
void data_lcd(unsigned char address); //�������� ������������� ��������
void data_out_lcd(unsigned char address); //����� �������������� �������� �� �����
void write_temp_lcd(unsigned char address); //
void power_off_lcd(unsigned char address); //���������� �������
void heater_ground(unsigned char address); //��������� ��������� �������
void water_ground (unsigned char address); //��������� ��������� ������
void lcd_putnibble_i2c(unsigned char data, unsigned char nibble, unsigned char address); //�������� ��������� ������ � ������ ���������� (i2c)
void lcd_putbyte_i2c(unsigned char rs, unsigned char byte, unsigned char address); //�������� ����� ������ � ������ ���������� (i2c)
void lcd_putchar_i2c(unsigned char znak, unsigned char address); //����� ������� �� ��������� (i2c)
void lcd_gotoxy_i2c (unsigned char adr, unsigned char address); //��������� ������� �� ��������� ������� ���������� (i2c)
void lcd_putstring_i2c(char stroka[], unsigned char address); //����� ������ �������� �� ��������� (i2c)
void lcd_init_i2c(); //������������� ���������� (i2c)
void lcd_clear_i2c(unsigned char address); //������� ���������� (i2c)
void bin_lcd_i2c(unsigned char bin, unsigned char address); //����� ��������� ����� �� ��������� (i2c)
void dec_lcd_i2c(unsigned char dec, char rs, unsigned char address); //����� ����������� ����� �� ��������� (i2c)
void ascii_lcd_i2c(unsigned char hex, char rs, unsigned char address); //����� ������������������ ����� �� ��������� (i2c)

#endif /* _LCD_H_ */
