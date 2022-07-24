#ifndef BUTTONS_H_
#define BUTTONS_H_

const int dcdcButtonInputPin = 14;
const int plantButtonInputPin = 15;
const int lightButtonInputPin = 18;

const int buttonPressDuration = 100;

void init_Buttons();

void handle_Buttons();

void calc_press_dur(unsigned long *, unsigned long );

unsigned long elapsed_ms(unsigned long);


#endif