#ifndef SCREEN_UI_HPP
#define SCREEN_UI_HPP

#include <Arduino.h>
#include <Wire.h>
#include <LiquidCrystal_I2C.h>

// LCD configuration
#define LCD_ADDRESS   0x27
#define LCD_WIDTH     16
#define LCD_HEIGHT    2

// Step range options
#define STP_RNG_OPT   AUTO
#define STP_RNG_OPT2  1mA
#define STP_RNG_OPT3  10mA
#define STP_RNG_OPT4  100mA
#define STP_RNG_OPT5  1A

// Averaging options
#define STP_AVG_OPT   1
#define STP_AVG_OPT2  4
#define STP_AVG_OPT3  8
#define STP_AVG_OPT4  16
#define STP_AVG_OPT5  32

class screenUI {
public:
    screenUI();

    void arrow(int row, int col, bool show_hide, int type);
    void clearLcdLine(char line[], uint8_t len);
    void lcdUpdate(char str0[], char str1[]);
    bool updateUI(bool arrow_place);
    bool updateUI();
    void initUI();

    // Public buffers for LCD lines
    char Line0[LCD_WIDTH + 1];
    char Line1[LCD_WIDTH + 1];

    // Public menu arrays
    char start_menu[2][LCD_WIDTH] = {
        " SETUP",
        " MEASURE"
    };

    char setup_menu[2][LCD_WIDTH] = {
        " Range: ",
        " Average: "
    };

    char meas_menu[2][LCD_WIDTH] = {
        "I=100mA  Avg=32",
        " 100mOhms"
    };

    // Arrow symbols
    uint8_t symbol[8]  = {0x00, 0x04, 0x06, 0x1F, 0x06, 0x04, 0x00};
    uint8_t isymbol[8] = {0x1F, 0x1B, 0x19, 0x00, 0x19, 0x1B, 0x1F, 0x1F};

private:
    LiquidCrystal_I2C screen;
};

#endif // SCREEN_UI_H








// #define LCD_ADDRESS              0x27
// #define LCD_WIDTH                16
// #define LCD_HEIGHT               2
// #define STP_RNG_OPT              AUTO
// #define STP_RNG_OPT2             1mA
// #define STP_RNG_OPT3             10mA
// #define STP_RNG_OPT4             100mA
// #define STP_RNG_OPT5             1A
// #define STP_AVG_OPT              1
// #define STP_AVG_OPT2             4
// #define STP_AVG_OPT3             8
// #define STP_AVG_OPT4             16
// #define STP_AVG_OPT5             32


// // these two buffers are used for refreshing the LCD
// char Line0[LCD_WIDTH]; 
// char Line1[LCD_WIDTH];

// // these 2 dimensional arrays are for the predefined menu
// char start_menu[2][LCD_WIDTH]={" SETUP",
//     " MEASURE"};
// char setup_menu[2][LCD_WIDTH]={" Range: ",
//        " Average: "};
// char meas_menu[2][LCD_WIDTH]={"I=100mA  Avg=32",
//        " 100mOhms"};

// uint8_t symbol[8] = {0x00, 0x04, 0x06, 0x1F, 0x06, 0x04, 0x00}; //Arrow symbol
// uint8_t isymbol[8] = {0x1F, 0x1B, 0X19, 0X00, 0X19, 0X1B, 0X1F, 0X1F};// inverted arrow symbol

// void arrow (int row, int col, bool show_hide, int type);
// void clearLcdLine(char line[], uint8_t len);
// void LcdUpdate(char str0[],char str1[]);
// bool update_UI(bool arrow_place);
// void  init_UI();
