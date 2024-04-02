#include "dip_config.h"



void setup_dip() {
    shift_register_setup();
    read_dip();
}


void read_dip() {
    dip_config = read_shift_register();

    // Serial.println("Pin States:");
    // Serial.println(dip_config, BIN);
    
    set_midi_channel(dip_config);
    set_tuning_mode(dip_config);

}


void set_midi_channel(byte dip_config) {
    byte extracted_channel = (dip_config & DIP_ADDRESS_MASK) >> 4;
    synth_midi_channel = extracted_channel + 1;
}


void set_tuning_mode(byte dip_config) {
    byte extracted_mode = (dip_config & DIP_TUNING_MASK) >> 3;
    tuning_enabled = extracted_mode;
}
