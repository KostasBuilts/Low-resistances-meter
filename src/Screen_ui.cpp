#include "screen_ui.hpp"

// Constructor
screenUI::screenUI()
    : screen(LCD_ADDRESS, LCD_WIDTH, LCD_HEIGHT) {}

/**
 * @brief displays or hides an arrow charachter on a specified screen cell. Must be called after the lcdUpdate or else the cursor will be written over
 * @param int row
 * @param int column
 * @param bool visible
 * @param int type (0 for normal, 1 for inverted)
 */
void screenUI::arrow(int row, int col, bool show_hide, int type) {
    if (show_hide) {
        screen.setCursor(row, col);
        screen.write(type);
    }
}

/**
 * @brief clears a line in an char array with specified lenght
 * @param char[] line
 * @param uint8_t len
 */
void screenUI::clearLcdLine(char line[], uint8_t len) {
    for (int i = 0; i < len; i++) {
        line[i] = 0x20;
    }
}

/**
 * @brief updates the lcd screen based on the strings provided
 * @param char[] line1
 * @param char[] line2
 */
void screenUI::lcdUpdate(char str0[], char str1[]) {
    screen.setCursor(0, 0);
    screen.print(str0);
    screen.setCursor(0, 1);
    screen.print(str1);
}

// High-level UI update method with arrow
bool screenUI::updateUI(bool arrow_place) {
    lcdUpdate(Line0, Line1);
    arrow(0, arrow_place, true, 0);
    return true;
}

// High-level UI update method without arrow
bool screenUI::updateUI() {
    lcdUpdate(Line0, Line1);
    arrow(0, 0, false, 0);
    return true;
}

// Initialize the LCD screen and UI
void screenUI::initUI() {
    if (!screen.init()) {
        Serial.println("Can't connect to the ADC");
    }
    screen.backlight();
    screen.createChar(0, symbol);
    screen.createChar(1, isymbol);
    clearLcdLine(Line0, LCD_WIDTH);
    clearLcdLine(Line1, LCD_WIDTH);
}
