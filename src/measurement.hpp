#include <Arduino.h>
#include <ADS1115_WE.h>

// ADC and Pin Definitions
#define ADC_ADDRESS     0x48
#define UP              6
#define DOWN            7
#define SELECT          8
#define RANGE_1A        2
#define RANGE_100mA     3
#define RANGE_10mA      4
#define RANGE_1mA       5

class Measure {
public:
    Measure();

    float readChannel(ADS1115_MUX channel);
    float avg(uint8_t avg_count);
    float calculate_mOhms(float reading, uint8_t range);
    void range_select(uint8_t range);
    void pins_init();
    void init_measurement();

    ADS1115_WE adc;
};
