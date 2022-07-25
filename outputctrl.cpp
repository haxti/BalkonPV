#include "Arduino.h"
#include <NTPClient.h>
#include "outputctrl.h"

extern NTPClient timeClient;

extern float batteryVoltage;
extern float solarVoltage;

bool dcdcOut = false;
bool plantOut = false;
bool lightOut = false;

void init_Outputctrl()
{
  pinMode(dcdcOutputPin, OUTPUT);
  pinMode(plantOutputPin, OUTPUT);
  pinMode(lightOutputPin, OUTPUT);

  digitalWrite(dcdcOutputPin, LOW);
  digitalWrite(plantOutputPin, LOW);
  digitalWrite(lightOutputPin, LOW);
  Serial.println("Outputctrl init ok");
}


void handle_Outputctrl()
{
  handle_light();
  handle_plant();
  handle_dcdc();
}


void handle_light()
{
  bool batteryOk = false;
  bool sunDown = false;
  bool currentState = lightOut;
  

  if(lightOnMinHour > lightOffHour)   // lightOffHour is on the next day
  {
    if(timeClient.getHours() >= lightOnMinHour || timeClient.getHours() <= lightOffHour)
      lightOut = true;
    else
      lightOut = false;
  }

  if(lightOnMinHour < lightOffHour)   // lightOffHour is on the same day
  {
    if(timeClient.getHours() >= lightOnMinHour && timeClient.getHours() <= lightOffHour)
      lightOut = true;
    else
      lightOut = false;
  }

  if(solarVoltage <= lightOnSolarVoltage)
    sunDown = true;
  else
    sunDown = false;

  if(batteryVoltage >= lightMinVoltage)
    batteryOk = true;
  else if(currentState && batteryVoltage >= lightCritVoltage)
    batteryOk = true;
  else
    batteryOk = false;

  if(batteryOk && sunDown && lightOut)
  {
    if(!currentState)
      Serial.println("Light criteria met.");
    lightOut = true;
  } else
  {
    if(currentState)
    {
      Serial.println("Light criteria not met anymore.");
    }
    lightOut = false;
  }

  switchOutput(lightOutputPin, lightOut);
}

void handle_plant()
{
  bool batteryOk = false;
  bool currentState = plantOut;
  
  if(plantOnHour > plantOffHour)   // plantOffHour is on the next day
  {
    if(timeClient.getHours() >= plantOnHour || timeClient.getHours() <= plantOffHour)
      plantOut = true;
    else
      plantOut = false;
  }

  if(plantOnHour < plantOffHour)   // plantOffHour is on the same day
  {
    if(timeClient.getHours() >= plantOnHour && timeClient.getHours() <= plantOffHour)
      plantOut = true;
    else
      plantOut = false;
  }

  if(batteryVoltage >= plantMinVoltage)
    batteryOk = true;
  else if(currentState && batteryVoltage >= plantCritVoltage)
    batteryOk = true;
  else
    batteryOk = false;

  if(batteryOk && plantOut)
  {
    if(!currentState)
      Serial.println("Plant criteria met.");
    plantOut = true;
  } else
  {
    if(currentState)
    {
      Serial.println("Plant criteria not met anymore.");
      dcdcOut = false;      
    }
    plantOut = false;
  }

  switchOutput(plantOutputPin, plantOut);
}

void handle_dcdc()
{
  if(plantOut)
    dcdcOut = true;

  switchOutput(dcdcOutputPin, dcdcOut);
}

void switchOutput(int pin, bool state)
{
  if(digitalRead(pin) == state)
    return;

  if(state)
  {
    Serial.println("Powered " + pin);
    digitalWrite(pin, HIGH);
  } else
  {
    Serial.println("Shutdown " + pin);
    digitalWrite(pin, LOW);
  }
}
