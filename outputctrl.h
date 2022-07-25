#ifndef _OUTPUTCTRL_H_
#define _OUTPUTCTRL_H_

const int dcdcOutputPin = 4;
const int lightOutputPin = 13;
const int plantOutputPin = 12;


/* Light will be switched on if solar voltage is below threshold and time of day is between thresholds */
const int lightOnMinHour = 20;
const int lightOffHour = 1;
const float lightOnSolarVoltage = 10.0;
const float lightMinVoltage = 12.0;
const float lightCritVoltage = 11.6;


const int plantOnHour = 6;
const int plantOffHour = 11;
const float plantMinVoltage = 12.60;
const float plantCritVoltage = 12.0;

const int dcdcManualOnDuration = 30;
const int plantManualOnDuration = 60;
const int lightManualOnDuration = 60;

void init_Outputctrl();
void handle_Outputctrl();

void handle_light();
void handle_plant();
void handle_dcdc();

void switchOutput(int, bool );


#endif