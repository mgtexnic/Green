
#ifndef _VAR1_H_
#define _VAR1_H_

#include <stdint.h>

struct clock
{
	volatile uint8_t sec; //�������
	volatile uint8_t min; //������
	volatile uint8_t hours; //����
	volatile uint8_t day; // ���� ������
	volatile uint8_t chislo; // ���� ������
	volatile uint8_t month; //�����
	volatile uint8_t year; //���
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
	char C[3]; // ���� ������
	char Y[3]; //���
	
} time_1307;

typedef struct one_wire
{
	
	//�1, �2 - ������������ 1-� ������ (�����)
	uint8_t temp1[8]; //= {0xA0, 0x00, 0x04, 0x64, 0x05, 0x8E, 0xFF, 0x28};	//sensor �1 (A0|00|04|64|05|8E|FF|28)
	uint8_t temp2[8]; //= {0xB8, 0x00, 0x04, 0x64, 0x05, 0xB8, 0xFF, 0x28};	//sensor �2 (B8|00|04|64|05|B8|FF|28)
	//�3, �4 - ������������ 2-� ������ (�����)
	uint8_t temp3[8]; //= {0xB8, 0x00, 0x04, 0x64, 0x05, 0xB8, 0xFF, 0x28};	//sensor �3 (B8|00|04|64|05|B8|FF|28)
	uint8_t temp4[8]; //= {0xB8, 0x00, 0x04, 0x64, 0x05, 0xB8, 0xFF, 0x28};	//sensor �4 (B8|00|04|64|05|B8|FF|28)
	//�5, �6 - ������������ 3-� ������ (�����)
	uint8_t temp5[8]; //= {0xB8, 0x00, 0x04, 0x64, 0x05, 0xB8, 0xFF, 0x28};	//sensor �5 (B8|00|04|64|05|B8|FF|28)
	uint8_t temp6[8]; //= {0xB8, 0x00, 0x04, 0x64, 0x05, 0xB8, 0xFF, 0x28};	//sensor �6 (B8|00|04|64|05|B8|FF|28)
	//�7, �8 - ������������ 4-� ������ (�����)
	uint8_t temp7[8]; //= {0xB8, 0x00, 0x04, 0x64, 0x05, 0xB8, 0xFF, 0x28};	//sensor �7 (B8|00|04|64|05|B8|FF|28)
	uint8_t temp8[8]; //= {0xB8, 0x00, 0x04, 0x64, 0x05, 0xB8, 0xFF, 0x28};	//sensor �8 (B8|00|04|64|05|B8|FF|28)
	//�9, �10 - ������������ ��� ��������� ���������� ����������� �������
	uint8_t temp9[8]; //= {0xB8, 0x00, 0x04, 0x64, 0x05, 0xB8, 0xFF, 0x28};	//sensor �9 (B8|00|04|64|05|B8|FF|28)
	uint8_t temp10[8]; //= {0xB8, 0x00, 0x04, 0x64, 0x05, 0xB8, 0xFF, 0x28};	//sensor �10 (B8|00|04|64|05|B8|FF|28)
	//�11 - ������������ ��� ��������� �������� ����������� �������
	uint8_t temp11[8]; //= {0xB8, 0x00, 0x04, 0x64, 0x05, 0xB8, 0xFF, 0x28};	//sensor �11 (B8|00|04|64|05|B8|FF|28)
	
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

// ������ ������� - t = x*4096*2 (���. )

#define DAY 707788800 //(����� - tics)
#define TIME 24 //(24 ����)
#define REQ_LED 8192 //(1 �)
#define REQ_DOTS 4096 //(0.5 �)
#define REQ_WDT 8192 //(1 �)
#define REQ_SWICH 2048 //(0,25 �)
#define REQ_TIME 81920 //(10 �)
#define REQ_TIME_UPDATE 491520 //(2 c)
#define REQ_POWER 163840 //(20 �)
#define REQ_TAP 32768 //(4 �) 
#define REQ_SENS_TEMP 491520 //(60 �)

struct data
{
	volatile uint32_t nTime;
	volatile uint32_t Time;
	volatile uint16_t cDots; //������� ��������� �����
	volatile uint16_t cWdt; //����� ����������� �������
	volatile uint32_t StTime;
	volatile uint32_t cTimeUpdate;
	volatile uint32_t cTapPause; 
	volatile uint32_t cLedReq; //������� ����������
	volatile uint32_t cSwReq; //����� ������ � ����������
	volatile uint32_t cTimeReq; //����� RTC
	volatile uint32_t cPowReq; //�������� ���������� ���� ����� � �����������
	volatile uint32_t cWatReq; //�������� ���������� ���� ����� (������� ������ ������)
	volatile uint32_t cTimeWater; //�������� ���������� ���� ����� (������� ����������������� ������)
	volatile uint32_t cTap; //������ ��������/�������� �����
	volatile uint32_t cSenTReq; //����� �������� �����������
	
	//*******************************************************************//

	uint8_t high_byte;
	uint8_t low_byte;

	//*******************************************************************//

	uint8_t t_in_min; //= 0x00; //����� ����������� ���������� �����������
	uint8_t t_in_max; //= 0x10; //����� ������������ ���������� �����������
	uint8_t t_ground_min; //= 0x00; //����� ����������� ����������� �����
	uint8_t t_ground_max; //= 0x10; //����� ������������ ����������� �����
	uint8_t hours_water_start; //= 0x00; //����� ������ ������� (����)
	uint8_t min_water_start; //= 0x00; //����� ������ ������ (������)
	uint8_t hours_water_power; //= 0x00; //������������ ������ (����)
	uint8_t min_water_power; //= 0x00; //������������ ������ (������)
	uint8_t power_day;
	
	//*******************************************************************//

	uint8_t sec1; //�������
	uint8_t min1; //������
	uint8_t hours1; //����
	uint8_t sec; //�������
	uint8_t min; //������
	uint8_t hours; //����
	uint8_t day; // ���� ������
	uint8_t chislo; // ���� ������
	uint8_t month; //�����
	uint8_t year; //���
	char H[3];	//�������
	char M[3];	//������
	char S[3];	//����
	char C[3];	// ���� ������
	char Y[3];	//���
	char D[3];  //�������� ��������� ������ (���)

	//*******************************************************************//

	uint8_t water;	// ����� - on/off
	uint8_t heater; //�����������
	uint8_t power;

	//*******************************************************************//

	uint8_t m_power;  //�������� ������ ������� (������)
	uint8_t h_power;  //�������� ������ ������� (����)
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

	uint8_t light_level_day; //= 0x00; //������� ������������ - "����"
	uint8_t light_level_night; //= 0x00; //������� ������������ - "����"

	//*******************************************************************//

} setup;

extern termometr  ow_line;

#endif /* _VAR1_H_ */

 