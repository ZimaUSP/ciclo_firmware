#include "config.hpp"
#include "soc/sens_reg.h" // needed for manipulating ADC2 control register
//uint64_t reg_b; // Used to store ADC2 control register
#define PIN 15 // Substitute xx with your ADC2 Pin number
int value;

uint32_t adc_register;
uint32_t wifi_register;

#include <WiFi.h>
#include <WiFiClient.h>
#include <WebServer.h>
#include <ESPmDNS.h>

const char* ssid = "sem wifi";
const char* password = "12345678";

#include "Joystick.hpp"

Joystick *obj;

int readPot() {
  WRITE_PERI_REG(SENS_SAR_READ_CTRL2_REG, adc_register); // Wifi with ADC2 on ESP32 workaround.
  SET_PERI_REG_MASK(SENS_SAR_READ_CTRL2_REG, SENS_SAR2_DATA_INV);// Wifi with ADC2 on ESP32 workaround.

  int value = analogRead(PIN);
  
  WRITE_PERI_REG(SENS_SAR_READ_CTRL2_REG, wifi_register); // Wifi with ADC2 on ESP32 workaround.
  return value;
}

void setup() {
  Serial.begin(9600);

  
  adc_register = READ_PERI_REG(SENS_SAR_READ_CTRL2_REG); // Wifi with ADC2 on ESP32 workaround.
  WiFi.mode(WIFI_STA);
  WiFi.begin(ssid, password);
  wifi_register = READ_PERI_REG(SENS_SAR_READ_CTRL2_REG);

  obj = new Joystick(PIN, adc_register, wifi_register);
}


void loop() {
  Serial.print(obj->get_power());
  Serial.print(" -> ");
  if (obj->left()) {
    Serial.println("left");
  } else if (obj->right()) {
    Serial.println("right");
  } else if(obj->middle()) {
    Serial.println("middle");
  }
  //Serial.println(readPot());
  delay(2);
}