#include "measure.h"
#include <Wire.h>
#include <INA219_WE.h>
#include <Adafruit_INA260.h>

INA219_WE ina219_solar(I2C_ADDRESS_SOLAR);
Adafruit_INA260 ina260_battery = Adafruit_INA260();

extern float batteryVoltage;
extern float solarVoltage;

void init_Measure()
{
  Wire.begin();
  init_INA219();
  init_INA260();
  Serial.println("Measure init ok");
}

void init_INA219()
{
  if(!ina219_solar.init()){
    Serial.println("INA219 Solar not connected!");
    return;
  }

  ina219_solar.setADCMode(BIT_MODE_12); // choose mode and uncomment for change of default
  ina219_solar.setPGain(PG_320); // choose gain and uncomment for change of default
  ina219_solar.setCorrectionFactor(10); // insert your correction factor if necessary
  //ina219_solar.setShuntVoltOffset_mV(0.33);
}

void init_INA260()
{
    if (!ina260_battery.begin()) {
    Serial.println("Couldn't find INA260 chip");
    return;
    }

}


void handle_Measure()
{
  measure_INA219();
  Serial.println("\n------------\n");
  measure_INA260();
}

void measure_INA219()
{
  float shuntVoltage_mV_sol = 0.0;
  float loadVoltage_V_sol = 0.0;
  float busVoltage_V_sol = 0.0;
  float current_mA_sol = 0.0;
  float power_mW_sol = 0.0; 
  //bool ina219_overflow = false;
  
  shuntVoltage_mV_sol = ina219_solar.getShuntVoltage_mV();
  busVoltage_V_sol = ina219_solar.getBusVoltage_V();
  current_mA_sol = ina219_solar.getCurrent_mA();
  power_mW_sol = ina219_solar.getBusPower();
  //loadVoltage_V_sol  = busVoltage_V_sol + (shuntVoltage_mV_sol/1000);
  //ina219_overflow = ina219.getOverflow();

  Serial.print("Shunt Voltage_sol [mV]: "); Serial.println(shuntVoltage_mV_sol);
  Serial.print("Bus Voltage_sol [V]: "); Serial.println(busVoltage_V_sol);
  //Serial.print("Load Voltage_sol [V]: "); Serial.println(loadVoltage_V_sol);
  Serial.print("Current_sol [mA]: "); Serial.println(current_mA_sol);
  Serial.print("Bus Power_sol [mW]: "); Serial.println(power_mW_sol);


  solarVoltage = busVoltage_V_sol;
}


void measure_INA260()
{
    Serial.print("Current: ");
  Serial.print(ina260_battery.readCurrent());
  Serial.println(" mA");

  Serial.print("Bus Voltage: ");
  Serial.print(ina260_battery.readBusVoltage());
  Serial.println(" mV");

  Serial.print("Power: ");
  Serial.print(ina260_battery.readPower());
  Serial.println(" mW");

  batteryVoltage = ina260_battery.readBusVoltage();
}