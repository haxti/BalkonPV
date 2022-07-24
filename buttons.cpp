#include "buttons.h"
#include "Arduino.h"

unsigned long dcdcdur = 0;
unsigned long plantdur = 0;
unsigned long lightdur = 0;

extern bool dcdcButtonInputState;
extern bool plantButtonInputState;
extern bool lightButtonInputState;

void init_Buttons()
{
  pinMode(dcdcButtonInputPin, INPUT_PULLUP);
  pinMode(plantButtonInputPin, INPUT_PULLUP);
  pinMode(lightButtonInputPin, INPUT_PULLUP);
  Serial.println("Button init ok");
}

void handle_Buttons()
{
  static unsigned long prevMillis = 0;

  if(!digitalRead(dcdcButtonInputPin))
  {
    //Serial.println("DCDC pressed...");
    calc_press_dur(&dcdcdur, prevMillis);
  }


  if(!digitalRead(plantButtonInputPin))
  {
    //Serial.println("Plant pressed...");
    calc_press_dur(&plantdur, prevMillis);
  }


  if(!digitalRead(lightButtonInputPin))
  {
    //Serial.println("Light pressed...");
    calc_press_dur(&lightdur, prevMillis);
  }


  if(dcdcdur >= buttonPressDuration && digitalRead(dcdcButtonInputPin))
  {
    dcdcdur = 0;
    dcdcButtonInputState = 1;
    Serial.println("Button DCDC pressed ...");
  }

  if(lightdur >= buttonPressDuration && digitalRead(lightButtonInputPin))
  {
    lightdur = 0;
    lightButtonInputState = 1;
    Serial.println("Button Light pressed ...");
  }

  if(plantdur >= buttonPressDuration && digitalRead(plantButtonInputPin))
  {
    plantdur = 0;
    plantButtonInputState = 1;
    Serial.println("Button Plant pressed ...");
  }
  prevMillis = millis();
}

void calc_press_dur(unsigned long *duration, unsigned long prevMillis)
{
  if(!*duration)
  {
    *duration = 1;
  }
  else
  {
    *duration += elapsed_ms(prevMillis);
  }
}


unsigned long elapsed_ms(unsigned long prevMillis)
{
  unsigned long elapsedMillis = 0;
  unsigned long curMillis = millis();

  if(prevMillis > curMillis)
  {
    elapsedMillis = 4294967295 - prevMillis;
    elapsedMillis += curMillis;
  } else
  {
    elapsedMillis = curMillis - prevMillis;
  }

  prevMillis = curMillis;
  return elapsedMillis;
}