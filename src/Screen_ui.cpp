#include "screen_ui.hpp"

extern Measure measure;

static unsigned long splashStart = 0;

// State variables
static int selectedMenuItem = 0;
static int selectedRange = 0;
static int selectedAvg = 0;
static int selectedMode = 0;
// Menu state machine
    enum AppState {
    SPLASH,
    MAIN_SCREEN,
    MENU_SCREEN,
    SET_RANGE,
    SET_AVERAGES,
    SET_MODE
    };

    enum AppState currentState;


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
 * @brief Debounces an input and returns only when pressed
 * @param input pin
 * @return returns input pin only while pressed
 */
uint8_t screenUI::button(int pin)
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

uint8_t screenUI::getInput() {
  return button(6) | button(7) | button(8);
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


void screenUI::showSplashScreen() {
  snprintf(Line0, 16, "BEER electronics");
  snprintf(Line1, 16, " Milliohm Meter ");
  updateUI();
  delay(100);
  splashStart = millis();
  currentState = MAIN_SCREEN;
  clearLcdLine(Line0,LCD_WIDTH);
  clearLcdLine(Line1,LCD_WIDTH);
  lcdUpdate(Line0,Line1);
}

// Handles UI state transitions
void screenUI::handleUI() {
  uint8_t input = getInput();

  switch (currentState) {
    case MAIN_SCREEN:
      if (millis() - splashStart < 100) break;
      snprintf(Line0, 16, "R:%0.2f mOhm", measure.get_resistance());
      snprintf(Line1, 16, "A:%d %s", avgOptions[selectedAvg], modeOptions[selectedMode]);
      updateUI();
      if (input == 4) currentState = MENU_SCREEN;
      break;

    case MENU_SCREEN:
      snprintf(Line0, 16, "Range");
      snprintf(Line1, 16, "Averages");
      if (selectedMenuItem == 2) {
        snprintf(Line0, 16, "Mode");
        snprintf(Line1, 16, "Back");
      }
      updateUI(false);
      arrow(0, 0, selectedMenuItem % 2 == 0, 0);
      arrow(1, 0, selectedMenuItem % 2 == 1, 0);

      if (input == 3) selectedMenuItem = (selectedMenuItem + 1) % 4;
      if (input == 2) selectedMenuItem = (selectedMenuItem + 3) % 4;
      if (input == 4) {
        switch (selectedMenuItem) {
          case 0: currentState = SET_RANGE; break;
          case 1: currentState = SET_AVERAGES; break;
          case 2: currentState = SET_MODE; break;
          case 3: currentState = MAIN_SCREEN; break;
        }
      }
      break;

    case SET_RANGE:
      snprintf(Line0, 16, "Set Range:");
      snprintf(Line1, 16, "> %dmA", (int)(pow(10, selectedRange)));
      updateUI(true);
      if (input == 2 && selectedRange > 0) selectedRange--;
      if (input == 3 && selectedRange < 3) selectedRange++;
      if (input == 4) {
        measure.range_select(selectedRange);
        currentState = MAIN_SCREEN;
      }
      break;

    case SET_AVERAGES:
      snprintf(Line0, 16, "Averages:");
      snprintf(Line1, 16, "> %d", avgOptions[selectedAvg]);
      updateUI(true);
      if (input == 2 && selectedAvg > 0) selectedAvg--;
      if (input == 3 && selectedAvg < 4) selectedAvg++;
      if (input == 4) {
        measure.avg_count = avgOptions[selectedAvg];
        currentState = MAIN_SCREEN;
      }
      break;

    case SET_MODE:
      snprintf(Line0, 16, "Mode:");
      snprintf(Line1, 16, "> %s", modeOptions[selectedMode]);
      updateUI(true);
      if (input == 2 || input == 3) selectedMode ^= 1;
      if (input == 4) {
        currentState = MAIN_SCREEN;
      }
      break;

    default:
      break;
  }

  delay(100); // debounce
}