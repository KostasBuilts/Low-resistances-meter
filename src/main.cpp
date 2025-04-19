#include <Arduino.h>
#include <screen_ui.h>
#include "measurement.hpp"

screenUI ui;
Measure measure;

bool arrow_place=0;


/**
 * @brief Debounces an input and returns only when pressed
 * @param input pin
 * @return returns input pin only while pressed
 */
uint8_t button(int pin)
{
  #define KEY_ms      50
  unsigned long keyPressedTime;
  uint8_t c = 0;//the integer the will return the output
  if ((digitalRead(pin) == HIGH) && (millis() - keyPressedTime) > KEY_ms)//debounce routine
  {
    keyPressedTime = millis();
    c = pin;
  }
  return c;
}


void setup()
{
  Serial.begin(115200);
  //Wire.begin();
	/*inti_UI();
  inti_measurment();
  LcdUpdate(Line0,Line1);*/
}


void loop()
{
  //update_UI(arrow_place);  
}
