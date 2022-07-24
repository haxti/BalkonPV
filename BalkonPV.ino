#include <WiFi.h>
#include <ArduinoOTA.h>
#include <NTPClient.h>
#include "settings.h"
#include "buttons.h"
#include "outputctrl.h"
#include "measure.h"

WiFiUDP ntpUDP;
NTPClient timeClient(ntpUDP, "de.pool.ntp.org", 7200, 60000);

bool dcdcButtonInputState = false;
bool plantButtonInputState = false;
bool lightButtonInputState = false;

void init_OTA()
{
  ArduinoOTA.setPort(ota_port);
  ArduinoOTA.setHostname(hostname);
  ArduinoOTA.setPassword(ota_pw);

  ArduinoOTA
  .onStart([]() {
    String type;
    if (ArduinoOTA.getCommand() == U_FLASH)
      type = "sketch";
    else // U_SPIFFS
      type = "filesystem";

    // NOTE: if updating SPIFFS this would be the place to unmount SPIFFS using SPIFFS.end()
    Serial.println("Start updating " + type);
    //WebSerial.println("Starting OTA update");
  })
  .onEnd([]() {
    Serial.println("\nEnd");
    //WebSerial.println("\nEnd");
  })
  .onProgress([](unsigned int progress, unsigned int total) {
    static int percentage = 0;
    Serial.printf("Progress: %u%%\n", (progress / (total / 100)));
    if(percentage < (progress / (total / 100)))
    {
      percentage = (progress / (total / 100));
      //WebSerial.println(percentage);
    }
  })
  .onError([](ota_error_t error) {
    Serial.printf("Error[%u]: ", error);
    if (error == OTA_AUTH_ERROR) Serial.println("Auth Failed");
    else if (error == OTA_BEGIN_ERROR) Serial.println("Begin Failed");
    else if (error == OTA_CONNECT_ERROR) Serial.println("Connect Failed");
    else if (error == OTA_RECEIVE_ERROR) Serial.println("Receive Failed");
    else if (error == OTA_END_ERROR) Serial.println("End Failed");
    Serial.println("OTA Error");
  });

  ArduinoOTA.begin();
  Serial.println("OTA init ok");
}

void setup() {
  Serial.begin(115200);
  delay(200);

  Serial.println("Waiting for WiFi");
  WiFi.begin(ssid, psk);
  while (WiFi.waitForConnectResult() != WL_CONNECTED) {
    Serial.println("Connection Failed! Rebooting...");
    delay(2000);
    ESP.restart();
  }
  Serial.println(" ... OK!");

  init_OTA();
  timeClient.begin();
  init_Buttons();
  init_Outputctrl();
  init_Measure();
  Serial.println("Init done");
}

void loop() {
  static unsigned long prevMillis = 0;
  ArduinoOTA.handle();
  timeClient.update();
  handle_Buttons();
  handle_Outputctrl();
  


  if(elapsed_ms(prevMillis) > 30*1000)
  {
    Serial.println("Current Time: " + timeClient.getFormattedTime());

    handle_Measure();
    prevMillis = millis();
  }
}
