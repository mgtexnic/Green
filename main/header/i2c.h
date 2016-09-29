
#ifndef _I2C_H_
#define _I2C_H_


void start(void);
void restart(void);
void stop(void);
void stop_start(void);
void byte_write(unsigned char data);
unsigned char byte_read(unsigned char ask);
void Transmit_DS1307(unsigned char data, unsigned char reg);
unsigned char Receive_DS1307 (unsigned char reg);
void TWI_Transmit(unsigned char address, unsigned char data);


#endif /* _I2C_H_ */
