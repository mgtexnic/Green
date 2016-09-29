
#ifndef _VAR1_H_
#define _VAR1_H_

#include <stdint.h>

struct clock
{
	volatile uint8_t sec; //секунды
	volatile uint8_t min; //минуты
	volatile uint8_t hours; //часы
	volatile uint8_t day; // день недели
	volatile uint8_t chislo; // день мес€ца
	volatile uint8_t month; //мес€ц
	volatile uint8_t year; //год
	volatile uint8_t min_RW;
	volatile uint8_t hours_RW;
	volatile uint8_t day_RW;
	volatile uint8_t chislo_RW;
	volatile uint8_t month_RW;
	volatile uint8_t year_RW;
	uint8_t flag_t;
	uint32_t time;
	uint8_t a;
	char H[3];
	char M[3];
	char S[3];
	char C[3]; // день мес€ца
	char Y[3]; //год
	
} time_1307;

typedef struct one_wire
{
	
	//є1, є2 - термодатчики 1-й группы (земл€)
	uint8_t temp1[8]; //= {0xA0, 0x00, 0x04, 0x64, 0x05, 0x8E, 0xFF, 0x28};	//sensor є1 (A0|00|04|64|05|8E|FF|28)
	uint8_t temp2[8]; //= {0xB8, 0x00, 0x04, 0x64, 0x05, 0xB8, 0xFF, 0x28};	//sensor є2 (B8|00|04|64|05|B8|FF|28)
	//є3, є4 - термодатчики 2-й группы (земл€)
	uint8_t temp3[8]; //= {0xB8, 0x00, 0x04, 0x64, 0x05, 0xB8, 0xFF, 0x28};	//sensor є3 (B8|00|04|64|05|B8|FF|28)
	uint8_t temp4[8]; //= {0xB8, 0x00, 0x04, 0x64, 0x05, 0xB8, 0xFF, 0x28};	//sensor є4 (B8|00|04|64|05|B8|FF|28)
	//є5, є6 - термодатчики 3-й группы (земл€)
	uint8_t temp5[8]; //= {0xB8, 0x00, 0x04, 0x64, 0x05, 0xB8, 0xFF, 0x28};	//sensor є5 (B8|00|04|64|05|B8|FF|28)
	uint8_t temp6[8]; //= {0xB8, 0x00, 0x04, 0x64, 0x05, 0xB8, 0xFF, 0x28};	//sensor є6 (B8|00|04|64|05|B8|FF|28)
	//є7, є8 - термодатчики 4-й группы (земл€)
	uint8_t temp7[8]; //= {0xB8, 0x00, 0x04, 0x64, 0x05, 0xB8, 0xFF, 0x28};	//sensor є7 (B8|00|04|64|05|B8|FF|28)
	uint8_t temp8[8]; //= {0xB8, 0x00, 0x04, 0x64, 0x05, 0xB8, 0xFF, 0x28};	//sensor є8 (B8|00|04|64|05|B8|FF|28)
	//є9, є10 - термодатчики дл€ измерени€ внутренней температуры воздуха
	uint8_t temp9[8]; //= {0xB8, 0x00, 0x04, 0x64, 0x05, 0xB8, 0xFF, 0x28};	//sensor є9 (B8|00|04|64|05|B8|FF|28)
	uint8_t temp10[8]; //= {0xB8, 0x00, 0x04, 0x64, 0x05, 0xB8, 0xFF, 0x28};	//sensor є10 (B8|00|04|64|05|B8|FF|28)
	//є11 - термодатчики дл€ измерени€ наружней температуры воздуха
	uint8_t temp11[8]; //= {0xB8, 0x00, 0x04, 0x64, 0x05, 0xB8, 0xFF, 0x28};	//sensor є11 (B8|00|04|64|05|B8|FF|28)
	
	uint8_t z[11];
	uint8_t t[11];
	uint8_t i[11];
	
	uint8_t tmp;
	uint8_t znak;
	uint8_t index;
	
	uint8_t tmp8, tmp7, tmp6, tmp5, tmp4, tmp3, tmp2, tmp1, tmp0;
	uint8_t data7, data6, data5, data4, data3, data2, data1, data0;
	
	volatile uint8_t time_flag;
	volatile uint8_t wire_flag;
	volatile uint8_t presence1;
	volatile uint8_t presence2;
	volatile uint8_t flag;
	uint8_t n_bit;
	
	char g0[3];
	char g1[3];
	char g2[3];
	char g3[3];
	char in1[3];
	char in2[3];
	char out[3];
	
} termometr;

// расчет времени - t = x*4096*2 (сек. )

#define DAY 707788800 //(сутки - tics)
#define TIME 24 //(24 часа)
#define REQ_LED 8192 //(1 с)
#define REQ_DOTS 4096 //(0.5 с)
#define REQ_WDT 8192 //(1 с)
#define REQ_SWICH 2048 //(0,25 с)
#define REQ_TIME 81920 //(10 с)
#define REQ_TIME_UPDATE 491520 //(2 c)
#define REQ_POWER 163840 //(20 с)
#define REQ_TAP 32768 //(4 с) 
#define REQ_SENS_TEMP 491520 //(60 с)

struct data
{
	volatile uint32_t nTime;
	volatile uint32_t Time;
	volatile uint16_t cDots; //мигание двоеточи€ часов
	volatile uint16_t cWdt; //сброс сторожевого таймера
	volatile uint32_t StTime;
	volatile uint32_t cTimeUpdate;
	volatile uint32_t cTapPause; 
	volatile uint32_t cLedReq; //мигание светодиода
	volatile uint32_t cSwReq; //опрос кнопок и концевиков
	volatile uint32_t cTimeReq; //опрос RTC
	volatile uint32_t cPowReq; //контроль управлени€ реле двери и нагревател€
	volatile uint32_t cWatReq; //контроль управлени€ реле крана (счетчик начала полива)
	volatile uint32_t cTimeWater; //контроль управлени€ реле крана (счетчик продолжительности полива)
	volatile uint32_t cTap; //таймер открыти€/закрыти€ крана
	volatile uint32_t cSenTReq; //опрос датчиков температуры
	
	//*******************************************************************//

	uint8_t high_byte;
	uint8_t low_byte;

	//*******************************************************************//

	uint8_t t_in_min; //= 0x00; //порог минимальной внутренней температуры
	uint8_t t_in_max; //= 0x10; //порог максимальной внутренней температуры
	uint8_t t_ground_min; //= 0x00; //порог минимальной температуры земли
	uint8_t t_ground_max; //= 0x10; //порог максимальной температуры земли
	uint8_t hours_water_start; //= 0x00; //врем€ начала плолива (часы)
	uint8_t min_water_start; //= 0x00; //врем€ начала полива (минуты)
	uint8_t hours_water_power; //= 0x00; //длительность полива (часы)
	uint8_t min_water_power; //= 0x00; //длительность полива (минуты)
	uint8_t power_day;
	
	//*******************************************************************//

	uint8_t sec1; //секунды
	uint8_t min1; //минуты
	uint8_t hours1; //часы
	uint8_t sec; //секунды
	uint8_t min; //минуты
	uint8_t hours; //часы
	uint8_t day; // день недели
	uint8_t chislo; // день мес€ца
	uint8_t month; //мес€ц
	uint8_t year; //год
	char H[3];	//секунды
	char M[3];	//минуты
	char S[3];	//часы
	char C[3];	// день мес€ца
	char Y[3];	//год
	char D[3];  //интервал включени€ полива (дни)

	//*******************************************************************//

	uint8_t water;	// полив - on/off
	uint8_t heater; //нагреватель
	uint8_t power;

	//*******************************************************************//

	uint8_t m_power;  //обратный отсчет времени (минуты)
	uint8_t h_power;  //обратный отсчет времени (часы)
	uint8_t butt_var;
	uint8_t set_num;
	uint8_t number;
	uint8_t shift;
	uint8_t run0;
	uint8_t run1;
	uint8_t run2;
	uint8_t run3;
	uint8_t a;
	uint8_t n;
	uint8_t var_dots;
	uint8_t button;
	uint32_t Tstart;
	uint32_t Tpower;
	uint8_t SenseWater;
	uint8_t SenseHeater;
	uint8_t DoorSenseOpen;
	uint8_t DoorSenseClose;
	uint8_t data_h1;
	uint8_t data_h2;
	uint8_t data_l1;
	uint8_t data_l2;
	uint8_t frame_H;
	uint8_t frame_M;
	uint8_t butshift;
	uint8_t open_close;
	volatile uint8_t ButRewIn; 
	volatile uint8_t ButRewOut; 
	uint32_t var;
	volatile uint32_t tic;

	//*******************************************************************//

	uint8_t light_level_day; //= 0x00; //уровень освещенности - "день"
	uint8_t light_level_night; //= 0x00; //уровень освещенности - "ночь"

	//*******************************************************************//

} setup;

extern termometr  ow_line;

#endif /* _VAR1_H_ */

 