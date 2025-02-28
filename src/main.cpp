#include <Arduino.h>
#include <lcd_menu.cpp>

#define LCD_WIDTH   16
#define LCD_HEIGHT  2   
#define UP          6
#define DOWN        7
#define SELECT      8

void setup() 
{
    Serial.begin(115200);
    screen.init();
    screen.createChar(0, symbol);
    screen.createChar(1, isymbol);
  
    clearLcdLine(Line0,LCD_WIDTH);
    clearLcdLine(Line1,LCD_WIDTH);
  
    LcdUpdate(Line0,Line1);
    strncpy(&Line0[1],&menu[0][0],15);
    strncpy(&Line1[1],&menu[1][0],15);
    
    pinMode(UP, INPUT);
    pinMode(DOWN, INPUT);
    pinMode(SELECT, INPUT);
}

void loop() 
{

    LcdUpdate(Line0,Line1);
  
    //line0[3]=0x41;
    
    strncpy(&Line0[1],&menu[option][0],15);
    strncpy(&Line1[1],&menu[option+1][0],15);
  
  
    
  
  
    if (button(UP) == UP)
    {
      option -=1;
      arrow_place-=1;
    }
  
    if (button(DOWN) == DOWN)
    {
      option +=1;
      arrow_place+=1;
    }
  
  
   if(option>=3) option=3;
   if(option<=0) option=0;

}
