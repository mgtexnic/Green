
#ifndef _MAIN_H_
#define _MAIN_H_


void off_wdt(void); //��������� ����������� �������
void start_adc(void); //������ �������������� ���
void init_Data(void); //������������� ����������
void power_control(unsigned char address); //
void water_control(void); //
void system_setup(void); //������������� ���������� � ������� �������
void ReadRtc1307(void); //���������� �������� ������� � DS1307 (����, ������, �������)
void SelectTimeWater(void); //
void write_data(unsigned char a_lcd); //
void stop_interrupt0(void); //
void start_interrupt0(void); //

#endif /* _MAIN_H_ */