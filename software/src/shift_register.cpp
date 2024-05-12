#include "shift_register.h"



void shift_register_setup()
{
    pinMode(SR_LD_PIN, OUTPUT);
    pinMode(SR_CLKINH_PIN, OUTPUT);
    pinMode(SR_CLK_PIN, OUTPUT);
    pinMode(SR_QH_PIN, INPUT);
}


uint8_t SN74HC165_shiftIn(uint8_t dataPin, uint8_t clockPin, uint8_t bitOrder)
{
    uint8_t value = 0;
    uint8_t i;

    for (i = 0; i < 8; ++i) {
        if(bitOrder == LSBFIRST)
            value |= digitalRead(dataPin) << i;
        else
            value |= digitalRead(dataPin) << (7 - i);

        digitalWrite(clockPin, LOW);
        digitalWrite(clockPin, HIGH);
    }
    return value;
}


byte read_shift_register()
{
    digitalWrite(SR_CLKINH_PIN, HIGH);
    digitalWrite(SR_CLK_PIN, LOW);

    digitalWrite(SR_LD_PIN, LOW);
    delayMicroseconds(5);
    digitalWrite(SR_LD_PIN, HIGH);
    delayMicroseconds(5);

    digitalWrite(SR_CLK_PIN, HIGH);
    digitalWrite(SR_CLKINH_PIN, LOW);

    byte input_state = SN74HC165_shiftIn(SR_QH_PIN, SR_CLK_PIN, LSBFIRST);

    digitalWrite(SR_CLKINH_PIN, HIGH);
    digitalWrite(SR_CLK_PIN, LOW);

    return input_state;
}
