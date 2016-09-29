
#ifndef _CLOCK_H_
#define _CLOCK_H_


unsigned char dec_to_bcd(unsigned char data);
unsigned char read_sec(unsigned char reg);
unsigned char read_min(unsigned char reg);
unsigned char read_hours(unsigned char reg);
unsigned char read_day(unsigned char reg);
unsigned char read_chislo(unsigned char reg);
unsigned char read_month(unsigned char reg);
unsigned char read_year(unsigned char reg);
void write_sec (unsigned char data, unsigned char reg);
void write_min (unsigned char data, unsigned char reg);
void write_hours (unsigned char data, unsigned char reg);
void write_day (unsigned char data, unsigned char reg);
void write_chislo (unsigned char data, unsigned char reg);
void write_month (unsigned char data, unsigned char reg);
void write_year (unsigned char data, unsigned char reg);
void read_time(void);
void rewrite_time(void);
void time_bcd_to_hex(void);
void write_time(unsigned char sec, unsigned char min, unsigned char hours, unsigned char day,
unsigned char data, unsigned char month, unsigned char year);
void set_sec_1307(unsigned char sec, unsigned char address);
void set_min_1307(unsigned char min, unsigned char address);
void set_hours_1307(unsigned char hours, unsigned char address);
void set_day_1307(unsigned char day, unsigned char address);
void set_chislo_1307(unsigned char data, unsigned char address);
void set_month_1307(unsigned char month, unsigned char address);
void set_year_1307(unsigned char year, unsigned char address);
void clock_read (unsigned char sec, unsigned char min, unsigned char hours, unsigned char day,
unsigned char data, unsigned char month, unsigned char year, unsigned char address);
void set_clock(unsigned char hours, unsigned char min, unsigned char sec);
void set_calendar(unsigned char year, unsigned char month, unsigned char date, unsigned char day,
unsigned char hours, unsigned char min, unsigned char sec);
void on_clock(void);
void clock_power_out(unsigned char data);


#endif /* _CLOCK_H_ */
