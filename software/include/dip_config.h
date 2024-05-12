#pragma once

#include <Arduino.h>
#include "constants.h"
#include "parameters.h"
#include "shift_register.h"



void setup_dip(void);
void read_dip(void);
void set_midi_channel(byte dip_config);
void set_tuning_mode(byte dip_config);
