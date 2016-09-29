
#ifndef _MAIN_H_
#define _MAIN_H_


void off_wdt(void); //остановка сторожевого таймера
void start_adc(void); //запуск преобразовани€ ј÷ѕ
void init_Data(void); //инициализаци€ переменных
void power_control(unsigned char address); //
void water_control(void); //
void system_setup(void); //инициализаци€ встроенных и внешних модулей
void ReadRtc1307(void); //считывание чекущего времени с DS1307 (часы, минуты, секунды)
void SelectTimeWater(void); //
void write_data(unsigned char a_lcd); //
void stop_interrupt0(void); //
void start_interrupt0(void); //

#endif /* _MAIN_H_ */