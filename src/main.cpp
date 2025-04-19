#include <Arduino.h>
#include<ADS1115_WE.h> 
#include<Wire.h>
#include <LiquidCrystal_I2C.h>

#define ADC_ADDRESS              0x48
#define LCD_ADDRESS              0x27
#define LCD_WIDTH                16
#define LCD_HEIGHT               2   
#define UP                       6
#define DOWN                     7
#define SELECT                   8
#define RANGE_1A                 2
#define RANGE_100mA              3
#define RANGE_10mA               4
#define RANGE_1mA                5
#define STP_RNG_OPT              AUTO
#define STP_RNG_OPT2             1mA
#define STP_RNG_OPT3             10mA
#define STP_RNG_OPT4             100mA
#define STP_RNG_OPT5             1A
#define STP_AVG_OPT              1
#define STP_AVG_OPT2             4
#define STP_AVG_OPT3             8
#define STP_AVG_OPT4             16
#define STP_AVG_OPT5             32

ADS1115_WE adc = ADS1115_WE(ADC_ADDRESS);
LiquidCrystal_I2C screen(LCD_ADDRESS, LCD_WIDTH, LCD_HEIGHT);

/*bool start_menu = true;
bool setup_menu = false;
bool Measurement_menu = false;*/

// these two buffers are used for refreshing the LCD
char Line0[LCD_WIDTH]; 
char Line1[LCD_WIDTH];

// these 2 dimensional arrays are for the predefined menu
char start_menu[2][LCD_WIDTH]={" SETUP",
                                                      " MEASURE"};
char setup_menu[2][LCD_WIDTH]={" Range: ",
                                                         " Average: "};
char meas_menu[2][LCD_WIDTH]={"I=100mA  Avg=32",
                                                         " 100mOhms"};

uint8_t symbol[8] = {0x00, 0x04, 0x06, 0x1F, 0x06, 0x04, 0x00}; //Arrow symbol
uint8_t isymbol[8] = {0x1F, 0x1B, 0X19, 0X00, 0X19, 0X1B, 0X1F, 0X1F};// inverted arrow symbol

uint8_t arrow_place=0;

/**
 * @brief displays or hides an arrow charachter on a specified screen cell
 * @param int row
 * @param int column
 * @param bool visible
 * @param int type (0 for normal, 1 for inverted)
 */
void arrow (int row, int col, bool show_hide, int type)
{
  if (show_hide == true)
  {
    screen.setCursor(row, col);
    screen.write(type);
  }
}

/**
 * @brief clears a line in an char array with specified lenght
 * @param char[] line
 * @param uint8_t len
 */
void clearLcdLine(char line[], uint8_t len)
{
    for(int i=0;i<len;i++) line[i]=0x020;

}

/**
 * @brief updates the lcd screen based on the strings provided
 * @param char[] line1
 * @param char[] line2
 */
 void LcdUpdate(char str0[],char str1[])
{
    screen.setCursor(0,0);
    screen.print(str0);
    screen.setCursor(0,1);
    screen.print(str1);
} 

/*bool update_UI()
{
  strncpy(&Line0[1],&start_menu[0][0],15);
  strncpy(&Line1[1],&start_menu[1][0],15);
  LcdUpdate(Line0,Line1);
  arrow(0,arrow_place,true,0);
  return true;
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
 * @brief Initializes all the inputs and outputs. No Arguments
 */
void pins_init()
{
  pinMode(UP, INPUT);
  pinMode(DOWN, INPUT);
  pinMode(SELECT, INPUT);
  pinMode(RANGE_1A, OUTPUT);
  pinMode(RANGE_100mA, OUTPUT);
  pinMode(RANGE_10mA, OUTPUT);
  pinMode(RANGE_1mA, OUTPUT);
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

void setup()
{
  Serial.begin(115200);
  Wire.begin();

  try
	{
		if(screen.init() != true) throw false;
	}
	catch(bool error)
	{
		if(error == false) Serial.println("Error initializing screen");
		else Serial.println("Screen successfully initialized");
	}
  screen.backlight();
  screen.createChar(0, symbol);
  screen.createChar(1, isymbol);

  adc.setVoltageRange_mV(ADS1115_RANGE_6144);

  clearLcdLine(Line0,LCD_WIDTH);
  clearLcdLine(Line1,LCD_WIDTH);

  LcdUpdate(Line0,Line1);
}

void loop()
{
  try {
    //if(update_UI() == false) throw 1; // Throw an exception when a the UI didn't update successfully
  }
  catch (int error_code) {
    if(error_code == 1) 
      Serial.println("Error update the UI");
  }
  try {
    
    throw 2; // Throw an exception when a problem arise
  }
  catch (int error_code) {
    if(error_code == 2)
      Serial.println("Error reading from the ADC");
  }
}
