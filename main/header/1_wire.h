
#ifndef _1_WIRE_H_
#define _1_WIRE_H_


void ow_bit0(void);
void ow_bit1(void);
void reset_1_wire(void);
void presence_1_wire(void);
void read_presence(void);
void init_1_wire(void);
void ow_write_com(unsigned char com);
unsigned char ow_read_bit_nibble(unsigned char bit_nibble, unsigned char data, unsigned char r);
unsigned char ow_read_nibble(void);
unsigned char ow_read_byte(void);
void ow_select_temp_index(char ind, unsigned char address);
void ow_write_64(unsigned char temp[]);
void ow_read_data(unsigned char *data7, unsigned char *data6,unsigned char *data5,
unsigned char *data4, unsigned char *data3, unsigned char *data2,
unsigned char *data1, unsigned char *data0);
void ow_read_temp(unsigned char *tmp8, unsigned char *tmp7, unsigned char *tmp6,
unsigned char *tmp5, unsigned char *tmp4, unsigned char *tmp3,
unsigned char *tmp2, unsigned char *tmp1, unsigned char *tmp0);
void ow_read_select_temp(unsigned char *tmp8, unsigned char *tmp7, unsigned char *tmp6,
unsigned char *tmp5, unsigned char *tmp4, unsigned char *tmp3,
unsigned char *tmp2, unsigned char *tmp1, unsigned char *tmp0,
unsigned char temp[]);
void t_write(unsigned char temp[], unsigned char n);
void ow_read_number(void);
void ow_znak(unsigned char data, unsigned char address);
void ow_control_temp(void);
void ow_write_eeprom(unsigned char H, unsigned char L, unsigned char config, unsigned char temp[]);

#endif /* _1_WIRE_H_ */
