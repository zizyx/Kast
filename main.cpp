#include <util/delay.h>

// #include "board.h"
#include "uart.h"
// #include "climateControl.h"
// #include "systemTimer.h"
// #include "alarm.h"
// #include "nvm.h"
// #include "DS_3231.h"
// #include "BMP_280.h"
// #include "i2c.h"


int main() {
    uart            m_serial;
    m_serial.setCallback(rxCallback);

    addRxCallback(mainRxCb)

    // // I2C Devices
    // i2c             m_twi();
    // DS_3231         m_clock(m_twi);
    // BMP_280         m_baro_inside(m_twi);
    // BMP_280         m_baro_outside(m_twi);

    // // pwm 
    // pdm             m_fan;

    // // adc
    // adc             m_hygroCounter;

    // // Timers
    // alarm           m_alarm(m_clock);
    // systemTimer     m_timer(m_clock, m_alarm);

    // // nvm
    // nvm             m_storage;

    // Climate control
    // climateControl  m_cctl(m_baro, m_clock, m_storage);



    while(1){
        ///////////////////////////////////////////////////////////
        // EEPROM TEST
        // char string[81] = {"e\n\0"};
        // uint8_t array[80];
        // uint8_t array2[80];      
        // for (uint8_t i = 0; i < sizeof(array); ++i){
        //  array[i] = i;
        // }
        
        // nvm->nvmWriteBlock(5, array, sizeof(array));
        // nvm->nvmReadBlock(0, array2, sizeof(array2));
        
        // for (uint8_t j = 0; j < 10; j++)
        // {
        //  sprintf(string, "Memory is %d.\n", array2[j]);  
        //  m_debug->print(string);
        // }
        // _delay_ms(1000);
        ///////////////////////////////////////////////////////////

        // _delay_ms(10);
    }
    return 0;
}


