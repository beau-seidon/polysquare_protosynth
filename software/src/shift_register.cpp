#include "shift_register.h"



void shift_register_setup() {
    pinMode(SR_LD_PIN, OUTPUT);
    pinMode(SR_CLKINH_PIN, OUTPUT);
    pinMode(SR_CLK_PIN, OUTPUT);
    pinMode(SR_QH_PIN, INPUT);
}


byte read_shift_register() {
    // Write pulse to load pin
    digitalWrite(SR_LD_PIN, LOW);
    delayMicroseconds(5);
    digitalWrite(SR_LD_PIN, HIGH);
    delayMicroseconds(5);

    // Get data from 74HC165
    digitalWrite(SR_CLK_PIN, HIGH);
    digitalWrite(SR_CLKINH_PIN, LOW);
    byte incoming = shiftIn(SR_QH_PIN, SR_CLK_PIN, LSBFIRST);
    digitalWrite(SR_CLKINH_PIN, HIGH);

    return incoming;
}
