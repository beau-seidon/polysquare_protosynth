#pragma once

#include <Arduino.h>
#include "constants.h"
#include "parameters.h"
#include "shift_register.h"



void setup_dip(void);
void read_dip_switches(void);

void set_midi_channel(byte dip_config);
void set_tuning_mode(byte dip_config);
void set_duty_mode(byte dip_config);
void set_keyinvert_mode(byte dip_config);
void set_polyphony_mode(byte dip_config);
