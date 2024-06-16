#include "dip_config.h"



void setup_dip()
{
    shift_register_setup();
    read_dip_switches();
}



void read_dip_switches()
{
    dip_config = read_shift_register();

    // Serial.println("Pin States:");
    // Serial.println(dip_config, BIN);

    set_midi_channel(dip_config);
    set_tuning_mode(dip_config);
    set_duty_mode(dip_config);
    set_keyinvert_mode(dip_config);
    set_polyphony_mode(dip_config);
}



void set_midi_channel(byte dip_config)
{
    byte extracted_channel = (dip_config & DIP_ADDRESS_MASK) >> 4;
    synth_midi_channel = extracted_channel + 1;
}



void set_tuning_mode(byte dip_config)
{
    byte extracted_mode = (dip_config & DIP_TUNING_MASK) >> 3;
    tuning_enabled = extracted_mode;
}



void set_duty_mode(byte dip_config)
{
    byte extracted_mode = (dip_config & DIP_DUTY_MASK) >> 2;
    duty_mode = extracted_mode;
}



void set_keyinvert_mode(byte dip_config)
{
    byte extracted_mode = (dip_config & DIP_KEYINVERT_MASK) >> 1;
    keyinvert_mode = extracted_mode;
}



void set_polyphony_mode(byte dip_config)
{
    byte extracted_mode = (dip_config & DIP_KEYINVERT_MASK);
    polyphony_mode = extracted_mode;
}
