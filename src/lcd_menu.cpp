#include <Arduino.h>
#include <LiquidCrystal_I2C.h>
#include <Wire.h>

#define LCD_WIDTH   16
#define LCD_HEIGHT  2   
#define UP          4
#define DOWN        7
#define SELECT      2

LiquidCrystal_I2C screen(0x27, LCD_WIDTH, LCD_HEIGHT);

// these two buffers are used for refreshing the LCD
char Line0[LCD_WIDTH]; 
char Line1[LCD_WIDTH];

// this 2 dimension is the predefined menu
char menu[][LCD_WIDTH]={"My first line",
                        "My second line",
                        "My third line",
                        "My fourth line",
                        "               "};

uint8_t symbol[8] = {0x00, 0x04, 0x06, 0x1F, 0x06, 0x04, 0x00};
uint8_t isymbol[8] = {0x1F, 0x1B, 0X19, 0X00, 0X19, 0X1B, 0X1F, 0X1F};


int option = 0;
int arrow_place=0;


//**********************************************************
//
//**********************************************************
void arrow (int row, int col, bool show_hide, int type)
{
  if (show_hide == true)
  {
    screen.setCursor(row, col);
    screen.write(type);
  }
}


//********************************************************
//
//********************************************************
void clearLcdLine(char line[], uint8_t len)
{
    for(int i=0;i<len;i++) line[i]=0x020;

}


//********************************************************
//  
//********************************************************
void LcdUpdate(char str0[],char str1[])
{
    screen.setCursor(0,0);
    screen.print(str0);
    screen.setCursor(0,1);
    screen.print(str1);
}


//********************************************************
//
//********************************************************
uint8_t button(int pin)
{
  #define KEY_ms      500
  unsigned long keyPressedTime;
  uint8_t c = 0;//the integer the will return the output
  if ((digitalRead(pin) == HIGH) && (millis() - keyPressedTime) > KEY_ms)//debounce routine
  {
    keyPressedTime = millis();
    c = pin;
  }
  return c;
}