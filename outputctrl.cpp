#include "Arduino.h"
#include "outputctrl.h"

bool dcdcOut = false;
bool plantOut = false;
bool lightOut = false;

void init_Outputctrl()
{
  pinMode(dcdcOutputPin, OUTPUT);
  pinMode(plantOutputPin, OUTPUT);
  pinMode(lightOutputPin, OUTPUT);

  digitalWrite(dcdcOutputPin, HIGH);
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
  
  
}

void handle_plant()
{

}

void handle_dcdc()
{
  
}