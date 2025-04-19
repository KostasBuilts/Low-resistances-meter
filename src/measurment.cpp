#include<ADS1115_WE.h> 


#define ADC_ADDRESS              0x48   
#define UP                       6
#define DOWN                     7
#define SELECT                   8
#define RANGE_1A                 2
#define RANGE_100mA              3
#define RANGE_10mA               4
#define RANGE_1mA                5


ADS1115_WE adc = ADS1115_WE(ADC_ADDRESS);


/**
 * @brief Reads the ADC and converts the reeding to millivolts
 * @param ADS1115_MUX Channel
 * @return float voltage in millivolts
 */
float readChannel(ADS1115_MUX channel) {
    float voltage = 0.0;
    adc.setCompareChannels(channel);
    adc.startSingleMeasurement();
    while(adc.isBusy()){delay(0);}
    voltage = adc.getResult_mV();
    return voltage;
  }
  
  
  
  /**
   * @brief Avareges the ADC reading a specified amount up to 255
   * @param uint8_t avarege amount
   * @returns float avareged reading
   */
  float avg(uint8_t avg_count)
  {
    float temp=0;
    for(uint8_t i=0; i<=avg_count; i++)
    {
      temp+=readChannel(ADS1115_COMP_0_GND);
    }
    return temp/avg_count;
  }
  
  
  
  /**
   * @brief Calculates a mOhs value from a voltage reading on a set range
   * @param float voltage reading
   * @param uint8_t set range
   * @returns float calculated mOhms value
   */
  float calculate_mOhms(float reading, uint8_t range)
  {
    float out = 0;
    switch (range)
    {
      case 2: //1A range
        out = reading;
        break;
      case 3: //100mA range
        out = reading/10;
        break;
      case 4: //10mA range
        out = reading/100;
        break;
      case 5: //1mA range
        out = reading/1000;
        break;
    }
  }
  



/**
 * @brief Selects a current range
 * @param uint8_t range
*/
void  range_select(uint8_t range)
{
  switch (range)
  {
    case 2: //1A range
      digitalWrite(RANGE_1A, HIGH);
      digitalWrite(RANGE_100mA, LOW);
      digitalWrite(RANGE_10mA, LOW);
      digitalWrite(RANGE_1mA, LOW);
    break;
    case 3: //100mA range
      digitalWrite(RANGE_1A, LOW);
      digitalWrite(RANGE_100mA, HIGH);
      digitalWrite(RANGE_10mA, LOW);
      digitalWrite(RANGE_1mA, LOW);
      break;
    case 4: //10mA range
      digitalWrite(RANGE_1A, LOW);
      digitalWrite(RANGE_100mA, LOW);
      digitalWrite(RANGE_10mA, HIGH);
      digitalWrite(RANGE_1mA, LOW);
    break;
    case 5: //1mA range
      digitalWrite(RANGE_1A, LOW);
      digitalWrite(RANGE_100mA, LOW);
      digitalWrite(RANGE_10mA, LOW);
      digitalWrite(RANGE_1mA, HIGH);
    break;
  }
}

void inti_measurment()
{
    adc.setVoltageRange_mV(ADS1115_RANGE_6144);
}