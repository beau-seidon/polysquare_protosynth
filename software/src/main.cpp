#include <Arduino.h>

#include "constants.h"
#include "parameters.h"
#include "aux_functions.h"
#include "voice_functions.h"
#include "midi_handler.h"



void setup() {
    pinMode(LED_BUILTIN, OUTPUT);

    setup_pots();
    initialize_voices();
    initialize_tuning();
    initialize_midi();

    Serial.begin(SERIAL1_BAUD);
}


void loop() {
    read_midi();
    read_pots();
    show_activity(false);
}
