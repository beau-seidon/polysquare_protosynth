#include <Arduino.h>

#include "constants.h"
#include "parameters.h"
#include "dip_config.h"
#include "aux_functions.h"
#include "voice_functions.h"
#include "midi_handler.h"



void setup() {
    pinMode(LED_BUILTIN, OUTPUT);
    Serial.begin(SERIAL1_BAUD);
    Serial.println();

    setup_dip();
    setup_pots();
    initialize_voices();
    initialize_tuning();
    initialize_midi();
}


void loop() {
    read_midi();
    read_pots();
    show_activity(false);
}
