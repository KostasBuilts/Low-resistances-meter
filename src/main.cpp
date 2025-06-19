#include <Arduino.h>
#include <screen_ui.hpp>
#include "measurement.hpp"

screenUI ui;
Measure measure;

bool arrow_place=0;
bool measuring;

/*void auto_range()
{
  if(measuring != true)
  {
    for(int i=5; i>=0; i--)
    {
      measure.range_select(i);
      if(measure.avg(2) )
    }

  }
}*/

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
	ui.initUI();
  measure.init_measurement();
  ui.updateUI();
}


void loop()
{
  //update_UI(arrow_place);  
}
