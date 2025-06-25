#include "measurement.hpp"

// Constructor
Measure::Measure(): adc(ADC_ADDRESS) {}

/**
* @brief Reads the ADC and converts the reeding to millivolts
* @param ADS1115_MUX Channel
* @return float voltage in millivolts
*/
float Measure::readChannel(ADS1115_MUX channel) {
    float voltage = 0.0;
    adc.setCompareChannels(channel);
    adc.startSingleMeasurement();
    while (adc.isBusy()) {
        delay(0);
    }
    voltage = adc.getResult_mV();
    return voltage;
}

/**
* @brief Avareges the ADC reading a specified amount up to 255
* @param uint8_t avarege amount
* @returns float avareged reading
*/
float Measure::avg(uint8_t avg_count) {
    float temp = 0;
    for (uint8_t i = 0; i <= avg_count; i++) {
        temp += readChannel(ADS1115_COMP_0_GND);
    }
    return temp / avg_count;
}

/**
* @brief Calculates a mOhs value from a voltage reading on a set range
* @param float voltage reading
* @param uint8_t set range
* @returns float calculated mOhms value
*/
float Measure::calculate_mOhms(float reading, uint8_t range) {
    float out = 0;
    switch (range) {
        case 2: // 1A range
            out = reading;
            break;
        case 3: // 100mA range
            out = reading / 10;
            break;
        case 4: // 10mA range
            out = reading / 100;
            break;
        case 5: // 1mA range
            out = reading / 1000;
            break;
    }
    return out;
}

/**
 * @brief Selects a current range
 * @param uint8_t range
*/
void Measure::range_select(uint8_t range) {
    switch (range)
  {
    case RANGE_1A: //1A range
      digitalWrite(RANGE_1A, HIGH);
      digitalWrite(RANGE_100mA, LOW);
      digitalWrite(RANGE_10mA, LOW);
      digitalWrite(RANGE_1mA, LOW);
      current_range = RANGE_1A;
    break;
    case RANGE_100mA: //100mA range
      digitalWrite(RANGE_1A, LOW);
      digitalWrite(RANGE_100mA, HIGH);
      digitalWrite(RANGE_10mA, LOW);
      digitalWrite(RANGE_1mA, LOW);
      current_range = RANGE_100mA;
      break;
    case RANGE_10mA: //10mA range
      digitalWrite(RANGE_1A, LOW);
      digitalWrite(RANGE_100mA, LOW);
      digitalWrite(RANGE_10mA, HIGH);
      digitalWrite(RANGE_1mA, LOW);
      current_range = RANGE_10mA;
    break;
    case RANGE_1mA: //1mA range
      digitalWrite(RANGE_1A, LOW);
      digitalWrite(RANGE_100mA, LOW);
      digitalWrite(RANGE_10mA, LOW);
      digitalWrite(RANGE_1mA, HIGH);
      current_range = RANGE_1mA;
    break;
  }
}

/**
 * @brief Ititiates and avareges the readings and calculates the final value.
 * @returns returns the final mohm value.
 */
float Measure::get_resistance()
{
    return calculate_mOhms(avg(avg_count), current_range);
}

/**
 * @brief Initializes all the inputs and outputs. No Arguments
 */
void Measure::pins_init() {
    pinMode(UP, INPUT);
    pinMode(DOWN, INPUT);
    pinMode(SELECT, INPUT);
    pinMode(RANGE_1A, OUTPUT);
    pinMode(RANGE_100mA, OUTPUT);
    pinMode(RANGE_10mA, OUTPUT);
    pinMode(RANGE_1mA, OUTPUT);
}

// Initializes measurement setup and ADC
void Measure::init_measurement() {
    pins_init();
    if (!adc.init()) {
        Serial.println("Can't connect to the ADC");
    }
    adc.setVoltageRange_mV(ADS1115_RANGE_6144);
}
