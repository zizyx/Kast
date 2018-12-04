#include "commandHandler.h"


#define RX_BUFFER_SIZE  80
#define FLUSH           ';'

uint8_t rx[RX_BUFFER_SIZE];
uint8_t rxOfs = 0;
bool flushed = false;

void rxCallback(char *bytes, uint8_t len) {

    for (uint8_t i = 0; i < len; i++) {
        rx[Ofs++] = bytes[i];
        rx = (rx % RX_BUFFER_SIZE);

        if (bytes[i] == FLUSH)
    }
}

void mainRxCb(char *bytes, uint8_t len) {
    for (uint8_t i = 0; i < len; i++) {
        rxBytes[rxBytesOffset++] = bytes[i];
        rxBytesOffset = (rxBytesOffset % RX_BUFFER_SIZE);
    }
}

void checkRxBuffer() {

//Check of die ; er al is
// dan het juiste deel naar handleCommand(sturen)
    if (flushed) {
        handleCommand(rx, rxOfs)
    }

}

void handleCommand(char *data, uint8_t len) {

    // Test for the minimum length of a command
    if (len < 2)
        return;

    uint8_t cmd = data[1];
    uint8_t flags = data[0]
    bool rw = flags & 0x01 // Read is 0, write is 1

    switch (cmd) {
        case 0x01: //Do iets test main ofzo...
            if (rw == 0x01) {

            } else {

            }
            break;
        default:
            break; //Unkown command        
    }
}