#include <Arduino.h>
#include <screen_ui.hpp>
#include "measurement.hpp"

screenUI ui;
Measure measure;

bool arrow_place=0;
bool measuring;


void setup()
{
  Serial.begin(115200);
  //Wire.begin();
	ui.initUI();
  measure.init_measurement();
  ui.showSplashScreen();
}


void loop()
{
  ui.handleUI();
}
