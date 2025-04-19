#include <Arduino.h>
#include<Wire.h>
#include <LiquidCrystal_I2C.h>

#define LCD_ADDRESS              0x27
#define LCD_WIDTH                16
#define LCD_HEIGHT               2
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

LiquidCrystal_I2C screen(LCD_ADDRESS, LCD_WIDTH, LCD_HEIGHT);

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

/**
 * @brief displays or hides an arrow charachter on a specified screen cell. Must be called after the lcdUpdate or else the cursor will be written over
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

bool update_UI(bool arrow_place)
{

  LcdUpdate(Line0,Line1);
  arrow(0,arrow_place,true,0);
  return true;
}

void  inti_UI()
{
  screen.init();
  screen.backlight();
  screen.createChar(0, symbol);
  screen.createChar(1, isymbol);
  clearLcdLine(Line0,LCD_WIDTH);
  clearLcdLine(Line1,LCD_WIDTH);
}