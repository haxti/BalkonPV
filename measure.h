#ifndef _MEASURE_H_
#define _MEASURE_H_


#define I2C_ADDRESS_SOLAR 0x44
#define I2C_ADDRESS_BATTERY 0x40


void init_Measure();

void init_INA219();

void init_INA260();

void handle_Measure();

void measure_INA219();

void measure_INA260();


#endif